#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Channel.hpp"
#include "exceptions.hpp"
#include "commons.hpp"

#define BUFFER_SIZE 512
#define FAILED 	-1

struct t_recv_data
{
	t_recv_data(): buf_len(0), nbytes(0) {}

	char 		buf[BUFFER_SIZE];
	ssize_t		buf_len;
	ssize_t 	nbytes;
	std::string	input;
};

class Client
{

	public:

		typedef	std::pair<int, Client *>	pair;
		typedef std::map<int, Client *>		map;
		typedef std::vector<Client *>		vector;
		typedef map::iterator				iterator;
		typedef map::const_iterator			const_iterator;

		Client();
		Client( int fd );
		~Client();

		// methods
		ssize_t			read_data();
		void			join_channel(Channel *channel);
		void			leave_channel(std::string channel_name, std::string msg, int cmd);
		int				leave_joined_channels(std::string part_msg, int cmd, Channel::map &channel_book);
		void			clear_recv_data();
		void			set_input_to_quit();
		void			announce_new_nickname(std::string msg);

		// accessor
		void			set_authentication(std::string server_pass, std::string client_pass);
		void			set_username(std::string username);
		void			set_password(std::string password);
		void			set_realname(std::string realname);
		void			set_nickname(std::string nickname);
		void			set_ipstr(std::string ipstr);
		int				get_fd() const;
		bool			get_authentication() const;
		std::string		get_password() const;
		std::string		get_username() const;
		std::string		get_nickname() const;
		std::string		get_realname() const;
		std::string		get_ipstr() const;
		std::string		get_buf() const;
		std::string		get_input() const;

	private:

		Client( Client const & src );
		Client &operator=( Client const & rhs );

		int				_fd;
		bool			_authentication;
		std::string 	_password;
		std::string 	_nickname;
		std::string 	_username;
		std::string 	_realname;
		std::string		_ipstr;
		t_recv_data 	_recv_data;
		Channel::map	_joined_channels;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */
