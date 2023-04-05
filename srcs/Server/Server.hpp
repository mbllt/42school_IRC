#ifndef SERVER_HPP
# define SERVER_HPP

#include "commons.hpp"
#include "Client.hpp"
#include "ExecutionManager.hpp"

enum	e_event
{
	PENDING_CONNECTION,
	CONNECTION_LOST,
	DATA_RECEIVED,
	NO_EVENT
};

struct	t_server_info
{
	t_server_info(const char *prt) : port(prt) {}

	const char			*port;
	struct addrinfo		*servinfo;
	int					listening_socket;
};

class Server
{

	public :

		// member types
		typedef std::vector<struct pollfd>			pollfd_vector;
		typedef pollfd_vector::iterator				pollfd_iterator;

		// constructors & destructor
		Server(const char *port, const char *password);
		~Server();

		// member functions
		void	init();
		void	run();

	private :

		pollfd_vector			_pollfd;
		t_server_info			_server_info;					

		ExecutionManager		_exec;

		// init()
		void				_get_address_info();
		void				_get_listening_socket();
		int					_create_and_bind_socket(addrinfo *ptr);
		void				_add_socket_to_pollfd(int socket_fd);
		void				_listen_for_incoming_connections();
		// run()
		void				_poll_events();
		int					_find_event(pollfd current_pollfd);
		void				_accept_pending_connection();
		char*				_sockaddr_to_string(struct sockaddr_storage client_addr);
		void				_process_data(pollfd_iterator it);
		void				_close_connection(pollfd_iterator it);
		// error and log management
		void				_log(std::string log_msg);
		void				_err_log(std::string err_msg);
		void				_close_all_fds();
		static void			_signal_handler(int signum);

};

#endif /* ********************************************************** SERVER_HPP */