/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 15:37:24 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/14 10:39:10 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client() {}

Client::Client(int fd)
:
	_fd(fd),
	_authentication(false)
{}

Client::Client( const Client & src )
:
	_fd(src._fd),
	_authentication(src._authentication),
	_password(src._password),
	_nickname(src._nickname),
	_username(src._username),
	_realname(src._realname),
	_ipstr(src._ipstr),
	_recv_data(src._recv_data)
{}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client(){}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	_fd = rhs._fd;
	_authentication = rhs._authentication;
	_password = rhs._password;
	_nickname = rhs._nickname;
	_username = rhs._username;
	_realname = rhs._realname;
	_ipstr = rhs._ipstr;
	_recv_data = rhs._recv_data;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	o << "-------------------CLIENT--------------------" << std::endl;
	o << "fd = " << i.get_fd() << std::endl;
	o << "ipstr = " << i.get_ipstr() << std::endl;
	o << "password = " << i.get_password() << std::endl;
	o << "nickname = " << i.get_nickname() << std::endl;
	o << "username = " << i.get_username() << std::endl;
	o << "realname = " << i.get_realname() << std::endl;
	o << "---------------------------------------------" << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

ssize_t	Client::read_data()
{
	_recv_data.nbytes = recv(_fd, _recv_data.buf + _recv_data.buf_len, sizeof _recv_data.buf, 0);
	if (_recv_data.nbytes == FAILED)
		perror("Client: recv");
	else if (_recv_data.nbytes > 0)
	{
		_recv_data.buf_len += _recv_data.nbytes;
		_recv_data.buf[_recv_data.buf_len] = '\0';
		if (_recv_data.buf[_recv_data.buf_len - 1] == '\n') // if last char is '\n' then input is ready to be processed
		{
			_recv_data.input = _recv_data.buf;
			for (size_t i = 0; i < _recv_data.input.size(); i++)
				if (_recv_data.input[i] == '\r')
					_recv_data.input.erase(i, 1);
		}
	}
	return _recv_data.nbytes;
}

void	Client::join_channel(Channel *channel)
{
	_joined_channels.insert(Channel::pair(channel->get_name(), channel));
	channel->add_member(this);
	channel->remove_invitation(this);
}

int	Client::leave_joined_channels(std::string part_msg, int cmd, Channel::map &channel_book)
{
	Channel::iterator 	it;
	Channel				*channel;

	for (it = _joined_channels.begin(); it != _joined_channels.end(); it++)
	{
		channel = it->second;
		channel->erase_member(this, part_msg, cmd);
		if (this == channel->get_operator())
			channel->set_operator(NULL);
		if (channel->empty() == true)
		{
			channel_book.erase(channel->get_name());
			delete channel;
		}
	}
	_joined_channels.clear();
	return 0;
}

void	Client::leave_channel(std::string channel_name, std::string msg, int cmd)
{
	Channel	*channel = _joined_channels.find(channel_name)->second;

	channel->erase_member(this, msg, cmd);
	_joined_channels.erase(channel_name);
}

void	Client::clear_recv_data()
{
	_recv_data.input.clear();
	_recv_data.buf_len = 0;
	_recv_data.buf[0] = 0;
}

void Client::set_input_to_quit()
{
	_recv_data.input = "QUIT :connection lost\n";
}

void	Client::announce_new_nickname(std::string msg)
{
	Channel::iterator	chan_it;

	for (chan_it = _joined_channels.begin(); chan_it != _joined_channels.end(); chan_it++)
		chan_it->second->broadcast(NULL, msg);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void	Client::set_authentication(std::string server_pass, std::string client_pass)
{
	if (_nickname.empty() == 0
		&& _username.empty() == 0
		&& _password.empty() == 0)
		if (server_pass.compare(client_pass) == 0)
			_authentication = true;
}

void	Client::set_password(std::string password)
{
	_password = password;
}

void	Client::set_nickname(std::string nickname)
{
	_nickname = nickname;
}

void	Client::set_username(std::string username)
{
	_username = username;
}

void	Client::set_realname(std::string realname)
{
	realname.erase(0, 1);
	_realname = realname;
}

void	Client::set_ipstr(std::string ipstr)
{
	_ipstr = ipstr;
}

int	Client::get_fd() const
{
	return _fd;
}

bool	Client::get_authentication() const
{
	return _authentication;
}

std::string	Client::get_password() const
{
	return _password;
}

std::string	Client::get_nickname() const
{
	return _nickname;
}

std::string	Client::get_username() const
{
	return _username;
}

std::string	Client::get_realname() const
{
	return _realname;
}

std::string	Client::get_ipstr() const
{
	return _ipstr;
}

std::string	Client::get_buf() const
{
	return _recv_data.buf;
}

std::string	Client::get_input() const
{
	return _recv_data.input;
}

/* ************************************************************************** */