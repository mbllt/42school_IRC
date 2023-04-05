/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privateServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 10:57:01 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/14 10:15:02 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

# define BACKLOG 	10 
# define NO_TIMEOUT -1

/*
** ----------------------------------- INIT -----------------------------------
*/

void	Server::_get_address_info()
{
	int					status;
	struct addrinfo 	hints;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	status = getaddrinfo(NULL, _server_info.port, &hints, &_server_info.servinfo);
	if (status != 0)
		throw ServerExceptions::err_addrinfo(gai_strerror(status));
}

void	Server::_get_listening_socket()
{
	struct addrinfo*	ptr;

    // loop through all the results and bind to the first we can
    for (ptr = _server_info.servinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if (_create_and_bind_socket(ptr) > 0)
		{
			_add_socket_to_pollfd(_server_info.listening_socket);
			break;
		}
	}
	freeaddrinfo(_server_info.servinfo);
	if (ptr == NULL)
		throw ServerExceptions::err_socket();
}

int	Server::_create_and_bind_socket(addrinfo* ptr)
{
	int		opt = true;
	int		socket_fd = 0;

	if ((socket_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == FAILED)
	{
		perror("Server: socket");
		_err_log("Trying to create and bind another socket...");
		return FAILED;
	}
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == FAILED)
	{
		perror("setsockopt");
		_err_log("Trying to bind to the socket anyway...");
	}
	if (bind(socket_fd, ptr->ai_addr, ptr->ai_addrlen) == FAILED)
	{
		close(socket_fd);
		perror("Server: bind");
		_err_log("Trying to create and bind another socket...");
		return FAILED;
	}
	_server_info.listening_socket = socket_fd;
	return socket_fd;
}

void	Server::_add_socket_to_pollfd(int socket_fd)
{
	pollfd	new_pollfd = {socket_fd, POLLIN, 0};

	_pollfd.push_back(new_pollfd);
}

void	Server::_listen_for_incoming_connections()
{
	std::string		log_msg("Server: waiting for connections on port ");

	if (listen(_server_info.listening_socket, BACKLOG) == FAILED)
		throw ServerExceptions::err_listen(strerror(errno));
	else
	{
		log_msg.append(_server_info.port);
		_log(log_msg);
	}
}

/*
** ----------------------------------- RUN -----------------------------------
*/

void	Server::_poll_events()
{
	int				pfds;

	signal(SIGINT, _signal_handler);
	pfds = poll(_pollfd.data(), _pollfd.size(), NO_TIMEOUT);
	if (pfds == FAILED)
		throw ServerExceptions::err_poll(strerror(errno));
}

int	Server::_find_event(struct pollfd current_pollfd)
{
	if (current_pollfd.revents & (POLLIN | POLLHUP))
	{
		if (current_pollfd.fd == _server_info.listening_socket)
			return PENDING_CONNECTION;
		else
		{
			Client*	client = _exec.get_client(current_pollfd.fd);

			if (client == NULL)
				return CONNECTION_LOST;
			ssize_t	nbytes = client->read_data();

			if (nbytes > 0)
				return DATA_RECEIVED;
			else if (nbytes == 0)
				return CONNECTION_LOST;
		}
	}
	return NO_EVENT;
}

void	Server::_accept_pending_connection()
{
	int						new_fd;
	sockaddr_storage		client_addr;
    socklen_t 				addrlen;
	char* 					ipstr;
    
	addrlen = sizeof client_addr;
	new_fd = accept(_server_info.listening_socket, (struct sockaddr *)&client_addr, &addrlen);
	fcntl(new_fd, F_SETFL, O_NONBLOCK); // set fd to non blocking
	if (new_fd > 0)
	{
		_log("New connection accepted !");
		_add_socket_to_pollfd(new_fd);
		ipstr = _sockaddr_to_string(client_addr);
		_exec.init_client(new_fd, ipstr);
	}
	else
		perror("Server: accept");
}

char*	Server::_sockaddr_to_string(sockaddr_storage client_addr)
{
	static char	ipstr[INET6_ADDRSTRLEN];

	// deal with Clienth IPv4 and IPv6:
	if (client_addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&client_addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else
	{ // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&client_addr;
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	return ipstr;
}

void	Server::_process_data(pollfd_iterator it)
{
	Client *client = _exec.get_client(it->fd);

	_exec.run(client);
}

void	Server::_close_connection(pollfd_iterator it)
{
	int		fd = it->fd;
	Client	*client = _exec.get_client(fd);

	if (client != NULL)
	{
		client->set_input_to_quit();
		_exec.run(client);
	}
	_pollfd.erase(it);
	close(fd);
	std::cout << "Connection closed by client " << fd << std::endl;
}

/*
** --------------------------------- ERROR / LOG ---------------------------------
*/

void	Server::_log(std::string log_msg)
{
	std::cout << log_msg << std::endl;
}

void	Server::_err_log(std::string err_msg)
{
	std::cerr << err_msg << std::endl;
}

void	Server::_close_all_fds()
{
	pollfd_iterator	it;

	for (it = _pollfd.begin(); it != _pollfd.end(); it++)
		close((*it).fd);	
}
