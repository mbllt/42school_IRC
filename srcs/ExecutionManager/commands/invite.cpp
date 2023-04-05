#include "ExecutionManager.hpp"

int ExecutionManager::_invite_errors(Client *client, token_vector tokens) {
	std::string	cmd("INVITE");

	if (client->get_authentication() == false)
		return _send_rpl(client, ERR_NOTREGISTERED, 451);
	if (tokens.size() < 3)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);

	Channel::iterator chan_iterator = _find_chan_in_lowercase(tokens[2]);
	if (chan_iterator == _channel_book.end())
		return (_send_rpl(client, ERR_NOSUCHCHANNEL(tokens[2]), 403));
	Channel *channel = chan_iterator->second;

	if (channel->user_is_in_channel(client) == false)
		return (_send_rpl(client, ERR_NOTONCHANNEL(channel->get_name()), 442));
	if (channel->get_modes().find('i') != std::string::npos && channel->get_operator() != client)
		return (_send_rpl(client, ERR_CHANOPRIVSNEEDED(channel->get_name()), 482));
	if (_get_client_by_name(tokens[1]) == NULL)
		return (_send_rpl(client, ERR_NOSUCHNICK(tokens[1]), 401));
	if (channel->user_is_in_channel(_get_client_by_name(tokens[1])) == true)
		return (_send_rpl(client, ERR_USERONCHANNEL(tokens[1], channel->get_name()), 443));
	return (SUCCESS);
}

// INVITE nickname channel
int ExecutionManager::invite(Client *client, token_vector tokens) {
	if (int error = _invite_errors(client, tokens))
		return (error);

	Channel *channel = _find_chan_in_lowercase(tokens[2])->second;
	Client *invited_client = _get_client_by_name(tokens[1]);
	channel->add_to_invited(invited_client);

	//notify only inviting and invited clients
	_send_rpl(client, RPL_INVITING(client->get_nickname(), channel->get_name(), invited_client->get_nickname()), 341);
	_send_rpl(invited_client, MSG_INVITE(client->get_nickname(), invited_client->get_nickname(), channel->get_name()), 341);
	return SUCCESS;
}

/*
ERR_NOTONCHANNEL 
ERR_CHANOPRIVSNEEDED
ERR_NOSUCHNICK
ERR_USERONCHANNEL

RPL_INVITING
*/