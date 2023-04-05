#include "Server.hpp"

static volatile bool	server_running = true;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(const char* port, const char* password)
:
	_server_info(port),
	_exec(password)
{
	// if (port > 1024)
		// exit zobi ?
	// if password ... exit ?
	_pollfd.reserve(10);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	std::cout << "Server shutting down..." << std::endl;
	_close_all_fds();
}

/*
** ---------------------------------- MEMBERS ---------------------------------
*/

void	Server::init()
{
	_get_address_info();
	_get_listening_socket();
	_listen_for_incoming_connections();		
}

void	Server::run()
{
	while (server_running == true) 					// main loop
	{
		_poll_events(); 		// wait for events
		for (pollfd_iterator it = _pollfd.begin(); it < _pollfd.end(); it++)
		{
			switch (_find_event(*it))
			{
			case PENDING_CONNECTION:
				_log("PENDING CONNECTION");
				_accept_pending_connection();
				break;
			case CONNECTION_LOST:
				_log("CONNECTION LOST");
				_close_connection(it);
				break;
			case DATA_RECEIVED:
				_log("DATA RECEIVED");
				_process_data(it);
				break;
			case NO_EVENT:
				break;
			}
		}	
	}
}

void	Server::_signal_handler(int signum)
{
	if (signum == SIGINT)
		server_running = false;
}