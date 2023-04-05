#include "ExecutionManager.hpp"

int ExecutionManager::_kick_errors(Client *client, token_vector tokens, Channel *channel) {
	std::string	cmd("KICK");

	if (client->get_authentication() == false)
		return _send_rpl(client, ERR_NOTREGISTERED, 451);
	if (tokens.size() < 3)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (channel->get_operator() != client && client->get_fd() != _bot_fd)
		return (_send_rpl(client, ERR_CHANOPRIVSNEEDED(channel->get_name()), 482));
	return (SUCCESS);
}

int ExecutionManager::kick(Client *client, token_vector tokens) {
	Channel::iterator chan_iterator = _find_chan_in_lowercase(tokens[1]);
	Channel *channel = NULL;
	if (chan_iterator == _channel_book.end())
		return (_send_rpl(client, ERR_NOSUCHCHANNEL(tokens[1]), 403));
	else
		channel = chan_iterator->second;

	if (int error = _kick_errors(client, tokens, channel))
		return (error);

	token_vector targets = _split(tokens[2], ","); //for multiple user
	for (token_iterator it = targets.begin(); it != targets.end(); it++) 
	{
		chan_iterator = _find_chan_in_lowercase(tokens[1]);
		if (chan_iterator == _channel_book.end())
			return _send_rpl(client, ERR_NOSUCHCHANNEL(tokens[1]), 403);
		if (channel->user_is_in_channel(client) == false && client->get_fd() != _bot_fd)
			return (_send_rpl(client, ERR_NOTONCHANNEL(channel->get_name()), 442)); //if kicked myself, stops loops
		Client *target_client = _get_client_by_name(*it);
		if (channel->user_is_in_channel(target_client) == false) {
			_send_rpl(client, ERR_USERNOTINCHANNEL(tokens[2],channel->get_name()), 441);
			continue;
		}

		std::string kick_msg = MSG_KICK(client->get_nickname(), channel->get_name(), \
			target_client->get_nickname(), ((tokens.size() < 4) ? client->get_nickname() : tokens[3]));
		target_client->leave_channel(channel->get_name(), kick_msg, KICK);
		_remove_empty_channel(chan_iterator);
	}
	return SUCCESS;
}

/*
ERR_CHANOPRIVSNEEDED
ERR_NEEDMOREPARAMS
ERR_NOTONCHANNEL
ERR_USERNOTINCHANNEL          

Not for us :
	ERR_BADCHANMASK
*/