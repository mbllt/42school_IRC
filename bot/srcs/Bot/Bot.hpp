#ifndef BOT_HPP
# define BOT_HPP

# include "BotCommons.hpp"

#define BUFFER_SIZE 512
#define FAILED -1

struct t_recv_data
{
	t_recv_data(): nbytes(0) {}

	char 		buf[BUFFER_SIZE];
	ssize_t 	nbytes;
	std::string	input;
};

class Bot
{

	public:
		typedef std::vector<std::string>		token_vector;

		Bot();
		~Bot();

		void	init_bot(int fd);
		ssize_t	read_data();
		void	process_data();

	private:

		t_recv_data		_data;
		int				_fd;

		token_vector	_split(std::string const &buf, std::string sep);
		bool			_insult_is_found(std::string msg);
		void			_kick_the_malotru_out_of_chan(token_vector tokens);
		void			_kill_the_malotru(token_vector tokens);
		void			_send_random_answers(token_vector tokens);

};

#endif /* ************************************************************* BOT_H */