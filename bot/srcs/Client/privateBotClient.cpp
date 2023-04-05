#include "BotClient.hpp"

void	BotClient::_get_address_info()
{
	int					status;
	struct addrinfo 	hints;

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	status = getaddrinfo(NULL, _port, &hints, &_servinfo);
	if (status != 0)
		throw BotClientExceptions::err_addrinfo(gai_strerror(status));
}
void	BotClient::_get_receiving_socket()
{
	struct addrinfo*	ptr;

    // loop through all the results and bind to the first we can
    for (ptr = _servinfo; ptr != NULL; ptr = ptr->ai_next)
	{
		if (_connect_to_socket(ptr) > 0)
		{
			_pollfd->fd = _receiving_socket;
			_pollfd->events = POLLIN;
			break;
		}
	}
	freeaddrinfo(_servinfo);
	if (ptr == NULL)
		throw BotClientExceptions::err_socket();
}

int	BotClient::_connect_to_socket(addrinfo *ptr)
{
	int		socket_fd = 0;

	if ((socket_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == FAILED)
	{
		perror("BotClient: socket");
		return FAILED;
	}
	if (connect(socket_fd, ptr->ai_addr, ptr->ai_addrlen) == FAILED)
	{
		close(socket_fd);
		perror("BotClient: connect");
		return FAILED;
	}
	_receiving_socket = socket_fd;
	return socket_fd;
}

void	BotClient::_poll_events()
{
	int	pfds;

	signal(SIGINT, _signal_handler);
	pfds = poll(_pollfd, 1, NO_TIMEOUT);
	if (pfds == FAILED)
		throw BotClientExceptions::err_poll(strerror(errno));
}

int	BotClient::_find_event()
{
	ssize_t	nbytes =_bot.read_data();

	if (nbytes == 0)
		return CONNECTION_LOST;
	if (nbytes > 0)
		return DATA_RECEIVED;
	return FAILED;
}
