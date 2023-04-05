#include "ExecutionManager.hpp"

enum e_channel {
	INVITE_ONLY,
	CHANNEL_FOUND,
	USER_IS_IN_CHAN,
	BAD_CHANNEL_NAME,
	CHANNEL_NOT_FOUND
};

static int	find_channel(Channel::map &channel_book, std::string &channel_name, Client *client);
static void	create_new_channel(Channel::map &channel_book, Client *client, std::string &channel_name);

int ExecutionManager::join(Client *client, token_vector tokens) {
	std::string	cmd("JOIN");

	if (client->get_authentication() == false)
		return _send_rpl(client, ERR_NOTREGISTERED, 451);
	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (tokens[1] == "0")
		return client->leave_joined_channels(client->get_nickname(), PART, _channel_book);
	token_vector			channels = _split(tokens[1], ",");

	for (size_t  i = 0; i < channels.size(); i++) {
		switch (find_channel(_channel_book, channels[i], client)) {
			
		case BAD_CHANNEL_NAME:
			_send_rpl(client, ERR_BADCHANNAME(channels[i]), 479);
			break;
		case CHANNEL_NOT_FOUND:
			create_new_channel(_channel_book, client, channels[i]);
			_send_channel_infos(channels[i], client, MSG_JOIN(channels[i], client->get_nickname()));				
			break;
		case USER_IS_IN_CHAN:
			_send_rpl(client, ERR_USERONCHANNEL(client->get_nickname(), channels[i]), 443);
			break;
		case INVITE_ONLY:
			_send_rpl(client, ERR_INVITEONLYCHAN(channels[i]), 443);
			break;
		case CHANNEL_FOUND:
			client->join_channel(_channel_book.find(channels[i])->second); // add channel to client's joined_channel and client to channel's members
			_send_channel_infos(channels[i], client, MSG_JOIN(channels[i], client->get_nickname()));
			break;
		}
	}
	return SUCCESS;
}

// Process channel_name (to lower cases) and if it starts with '#', checking if the channel 
// exists or not with error management (USER_IS_IN_CHAN, BAD_CHANNEL_NAME, INVITE_ONLY)
static int	find_channel(Channel::map &channel_book, std::string &channel_name, Client *client) {
	for (std::string::size_type i = 0; i < channel_name.length(); ++i) // channel_name to lower
		channel_name[i] = std::tolower(channel_name[i]);
	if (channel_name[0] != '#' || channel_name.size() > 50)
		return BAD_CHANNEL_NAME;
	Channel::iterator	chan_it = channel_book.find(channel_name);
	Channel	*channel = chan_it->second;

	if (chan_it == channel_book.end())
		return CHANNEL_NOT_FOUND;
	if (channel->user_is_in_channel(client) == true)
		return USER_IS_IN_CHAN;
	if (channel->get_modes().find('i') != std::string::npos && channel->client_got_invited(client) == false)
		return INVITE_ONLY;
	return CHANNEL_FOUND;
}

// Create new channel, adding it to ExecutionManager::_channel_book &&
static void	create_new_channel(Channel::map &channel_book, Client *client, std::string &channel_name) {
	Channel	*new_channel = new Channel(channel_name);
	
	channel_book.insert(Channel::pair(channel_name, new_channel));
	client->join_channel(new_channel); // add channel to client's joined_channel and client to channel's members
	new_channel->set_operator(client);
}

// sending JOIN msg + RPL_TOPIC/NOTOPIC + RPL_NAMREPLY + RPL_ENDOFNAMES to client
int	ExecutionManager::_send_channel_infos(std::string channel_name, Client *client, std::string msg) {
	Channel 	*channel = _channel_book.find(channel_name)->second;
	std::string	channel_topic = channel->get_topic();
	std::string	client_nickname = client->get_nickname();
	
	// :llecoq!~llecoq@127.0.0.1 JOIN #baba
	if (_bot_fd != 0 && client->get_fd() != _bot_fd)
		send(_bot_fd, msg.c_str(), msg.size(), 0);
	channel->broadcast(NULL, msg);
	if (channel_topic.empty() == 1)
		_send_rpl(client, RPL_NOTOPIC(client_nickname, channel_name), 331);
	else
		_send_rpl(client, RPL_TOPIC(client_nickname, channel_name, channel_topic), 332);
	_send_rpl(client, RPL_NAMREPLY(channel_name, client_nickname, channel->list_members()), 353);
	_send_rpl(client, RPL_ENDOFNAMES(channel_name, client_nickname), 366);
	return SUCCESS;
}
