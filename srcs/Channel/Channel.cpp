#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel() : _name() {}

Channel::Channel(std::string name)
:
	_name(name),
	_modes("+")
{}

Channel::Channel( const Channel & src )
: 
	_name(src._name),
	_topic(src._topic),
	_modes(src._modes),
	_operator(src._operator),
	_members(src._members),
	_invited_clients(src._invited_clients)
{}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &				Channel::operator=( Channel const & rhs )
{
	_name = rhs._name;
	_topic = rhs._topic;
	_modes = rhs._modes;
	_operator = rhs._operator;
	_members = rhs._members;
	_invited_clients = rhs._invited_clients;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Channel const & i )
{
	o << "Channel name = " << i.get_name();
	o << "Channel topic = " << i.get_topic();
	o << "Channel modes = " << i.get_modes();
	return o;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

// set client to NULL to broadcast msg to the sending client itself too
void	Channel::broadcast(Client *client, std::string msg)
{
	for (size_t i = 0; i < _members.size(); i++)
		if (_members[i] != client)
		{
			if (send(_members[i]->get_fd(), msg.c_str(), msg.size(), 0) == FAILED)
				perror("Channel: send");
		}
}

void	Channel::add_member(Client *client)
{
	_members.push_back(client);
}

void	Channel::add_to_invited(Client *client) 
{
	_invited_clients.push_back(client);
}

void	Channel::remove_invitation(Client *client) 
{
	for (Client::vector::iterator it = _invited_clients.begin(); it != _invited_clients.end(); it++) 
	{
		if (client == *it) {
			_invited_clients.erase(it);
			return;
		}
	}
}

void	Channel::erase_member(Client *client, std::string msg, int cmd)
{
	std::vector<Client *>::iterator	it;

	for (it = _members.begin(); it < _members.end(); it++)
	{
		if (*it == client)
		{
			if (cmd == PART)
				broadcast(NULL, MSG_PART(_name, client->get_nickname(), msg));
			if (cmd == QUIT)
				broadcast(client, MSG_PART(_name, client->get_nickname(), msg));
			if (cmd == KICK)
				broadcast(NULL, msg);
			_members.erase(it);
			return ;
		}
	}
}

bool	Channel::user_is_in_channel(Client *client)
{
	for (size_t i = 0; i < _members.size(); i++)
		if (_members[i] == client)
			return true;
	return false;	
}

bool	Channel::client_got_invited(Client *client) {
	for (size_t i = 0; i < _invited_clients.size(); i++)
		if (_invited_clients[i] == client)
			return true;
	return false;
}


bool	Channel::empty()
{
	return _members.empty();
}

std::string	Channel::list_members()
{
	std::string	members;

	for (size_t i = 0; i < _members.size(); i++)
	{
		if (_members[i] == get_operator())
			members.append("@");
		members.append(_members[i]->get_nickname() + " ");
	}
	return members;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Channel::get_name() const
{
	return _name;
}

std::string	Channel::get_topic() const
{
	return _topic;
}

std::string	Channel::get_modes() const
{
	return _modes;
}

Client*	Channel::get_operator() const
{
	return _operator;
}

void	Channel::set_operator(Client *client)
{
	_operator = client;
}

void	Channel::set_modes(char c, size_t pos, char add_rmv)
{
	if (add_rmv == '+')
		_modes.append(1, c);
	else if (add_rmv == '-')
		_modes.erase(pos, 1);
}

void	Channel::set_topic(std::string topic)
{
	_topic = topic;
}

/* ************************************************************************** */