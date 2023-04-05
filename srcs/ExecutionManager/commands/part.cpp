#include "ExecutionManager.hpp"

int ExecutionManager::part(Client *client, token_vector tokens) {
	std::string	cmd("PART");
	std::string	nickname = client->get_nickname();
	std::string	part_msg;

	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (tokens.size() > 2)
		part_msg = tokens[2];
	else
		part_msg = nickname; 

	token_vector	channels = _split(tokens[1], ",");

	for (size_t  i = 0; i < channels.size(); i++) {
		Channel::iterator	chan_it = _channel_book.find(channels[i]);
		Channel				*channel = chan_it->second;

		if (chan_it == _channel_book.end()) {
			_send_rpl(client, ERR_NOSUCHCHANNEL(channels[i]), 403);
			continue;
		}
		if (channel->user_is_in_channel(client) == true) {
			client->leave_channel(channels[i], part_msg, PART);
			if (client == channel->get_operator())
				channel->set_operator(NULL);
			_remove_empty_channel(chan_it);
			continue;
		}
		_send_rpl(client, ERR_NOTONCHANNEL(channels[i]), 442);
	}
	return SUCCESS;
}
