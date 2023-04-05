#include "ExecutionManager.hpp"

int ExecutionManager::ping(Client *client, token_vector tokens) {
	std::string	msg("PONG ");

	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(tokens[0]), 461);
	msg.append(tokens[1]);
	msg.append(CRLF);
	return _send_rpl(client, msg, 0);
}