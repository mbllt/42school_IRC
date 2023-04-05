#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "commons.hpp"
#include "numeric_replies.hpp"

enum e_cmd {
	KICK,
	PART,
	QUIT
};

class Client;

class Channel
{

	public:
		typedef std::pair<std::string, Channel*>		pair;
		typedef std::map<std::string, Channel*>			map;
		typedef map::iterator							iterator;

		Channel(std::string name);
		~Channel();

		// accessors
		std::string	get_name() const;
		std::string	get_topic() const;
		std::string	get_modes() const;
		Client		*get_operator() const;
		void		set_operator(Client *client);
		void		set_modes(char c, size_t pos, char add_rmv);
		void		set_topic(std::string topic);
		
		// methods
		void		broadcast(Client *client, std::string msg);
		void		add_member(Client *client);
		void		add_to_invited(Client *client);
		void		remove_invitation(Client *client);
		void		erase_member(Client *client, std::string msg, int cmd);
		bool		user_is_in_channel(Client *client);
		bool		client_got_invited(Client *client);
		bool		empty();
		std::string	list_members();

	private:
		
		Channel();
		Channel( Channel const & src );
		Channel &operator=( Channel const & rhs );
		
		std::string				_name;
		std::string				_topic;
		std::string				_modes;
		Client					*_operator;
		std::vector<Client *>	_members;
		std::vector<Client *>	_invited_clients;

};

std::ostream &				operator<<( std::ostream & o, Channel const & i );

#include "Client.hpp"

#endif /* ********************************************************* CHANNEL_H */