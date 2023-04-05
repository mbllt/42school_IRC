#include "ExecutionManager.hpp"


int ExecutionManager::topic(Client *client, token_vector tokens) {
	std::string	cmd("TOPIC");

	if (client->get_authentication() == false)
		return _send_rpl(client, ERR_NOTREGISTERED, 451);
	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
		
	Channel::iterator chan_iterator = _channel_book.find(tokens[1]);
	if (chan_iterator == _channel_book.end())
		return _send_rpl(client, ERR_NOSUCHCHANNEL(tokens[1]), 403);

	//no need to be in channel to query topic or have op rights
	if (tokens.size() == 2)
		return (_topic_query(client, chan_iterator->second));

	return (_set_topic(client, chan_iterator->second, tokens[2]));
}

int ExecutionManager::_topic_query(Client *client, Channel *channel) {
	if (channel->get_topic().empty())
		return (_send_rpl(client, RPL_NOTOPIC(client->get_nickname(), channel->get_name()), 331));
	else
		return (_send_rpl(client, RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic()), 332));
}

//NB if topic is an empty string then we remove the topic
int ExecutionManager::_set_topic(Client *client, Channel *channel, std::string &topic) {
	std::string channel_name = channel->get_name();

	if (channel->user_is_in_channel(client) == false)
		return (_send_rpl(client, ERR_NOTONCHANNEL(channel_name), 442));

	if (channel->get_modes().find("t") != std::string::npos && channel->get_operator() != client)
		return (_send_rpl(client, ERR_CHANOPRIVSNEEDED(channel_name), 482)); //tested and works

	channel->set_topic(topic);
	channel->broadcast(NULL, RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic()));
	if (_bot_fd != 0 && client->get_fd() != _bot_fd) {
		std::string msg = RPL_TOPIC(client->get_nickname(), channel->get_name(), channel->get_topic());
		send(_bot_fd, msg.c_str(), msg.size(), 0);
	}
	return (332);
}

/*
Yes : 
	ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
	RPL_NOTOPIC                     RPL_TOPIC
	ERR_CHANOPRIVSNEEDED

No : ERR_NOCHANMODES
*/