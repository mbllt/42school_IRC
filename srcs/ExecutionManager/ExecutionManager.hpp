#ifndef EXECUTIONMANAGER_HPP
#define EXECUTIONMANAGER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include "commons.hpp"
#include "numeric_replies.hpp"
#include <fstream>

#define SUCCESS 0 // msg return for commands

class Client;

class ExecutionManager
{

	public:
		typedef std::vector<std::string>										token_vector;
		typedef token_vector::iterator											token_iterator;
		typedef int (ExecutionManager::*pf)(Client *, token_vector);
		typedef std::map<std::string, pf>										cmd_map;
		typedef cmd_map::iterator												cmd_iterator;
		typedef std::pair<std::vector<std::string>, std::vector<int> >			info_dest;

		ExecutionManager(std::string password);
		~ExecutionManager();

		//--------------------------------- ACCESSORS --------------------------------

		Client				*get_client(int fd) const;

		//--------------------------------- METHODS ----------------------------------
		void				init_client(int client_fd, const char *ipstr);
		int					run(Client *client);

	private:
		ExecutionManager();
		ExecutionManager(ExecutionManager const & src);
		ExecutionManager	&operator=(ExecutionManager const &rhs);

		cmd_map				_command_book;
		Client::map			_client_book;
		Channel::map		_channel_book;
		std::string			_password;
		int					_bot_fd;

		token_vector		_split(std::string const &buf, std::string sep);
		int					_find_fd_client_by_name(std::string nickname);
		int					_send_rpl(Client* client, std::string msg, int code);
		void				_remove_empty_channel(Channel::iterator chan_it);
		Channel::iterator 	_find_chan_in_lowercase(std::string channel_name);
		int					_send_welcome_msg(Client *client);

		// join
		int					_send_channel_infos(std::string channel_name, Client *client, std::string msg);

		// privmsg
		int					_err_privmsg_handling(Client *client, token_vector tokens, std::string rpl);
		std::string			_assemble_msg(token_vector token_msg);
		info_dest			_infos_dests(std::string str);
		bool				_dests_fd_valid(std::vector<int> dests);
		int					_msg_to_nicknames(Client *client, token_vector tokens, info_dest dests);
		int					_msg_to_channel(Client *client, token_vector tokens, Channel::iterator chan_it);

		// mode
		int					_display_infos_mode(Client *client, token_vector tokens, Channel* chan);
		int					_err_mode_handling(Client *client, token_vector tokens, Channel::iterator chan_it, Channel* chan);
		bool				_is_valid_mode_param(char c);
		std::string			_add_flags(Channel* chan, std::string new_flags);
		std::string			_remove_flags(Channel* chan, std::string new_flags);
		bool				_is_add_rmv(char c);

		//topic
		int 				_topic_query(Client *client, Channel *channel);
		int 				_set_topic(Client *client, Channel *channel, std::string &topic);

		//kick
		int 				_kick_errors(Client *client, token_vector tokens, Channel *channel);
		Client *			_get_client_by_name(std::string client_name);

		//invite
		int 				_invite_errors(Client *client, token_vector tokens);

		int					nick(Client *client, token_vector tokens);
		int					user(Client *client, token_vector tokens);
		int					join(Client *client, token_vector tokens);
		int					kick(Client *client, token_vector tokens);
		int					invite(Client *client, token_vector tokens);
		int					topic(Client *client, token_vector tokens);
		int					mode(Client *client, token_vector tokens);
		int					privmsg(Client *client, token_vector tokens);
		int					notice(Client *client, token_vector tokens);
		int					pass(Client *client, token_vector tokens);
		int					cap(Client *client, token_vector tokens);
		int					part(Client *client, token_vector tokens);
		int					ping(Client *client, token_vector tokens);
		int					quit(Client *client, token_vector tokens);
		int					bot(Client *client, token_vector tokens);
		int					kill(Client *client, token_vector tokens);

};

std::ostream				&operator<<(std::ostream &o, ExecutionManager const &i);

#endif
