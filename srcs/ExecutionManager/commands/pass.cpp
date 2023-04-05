#include "ExecutionManager.hpp"

int ExecutionManager::pass(Client *client, token_vector tokens) {
	std::string	cmd("PASS");

	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (client->get_authentication() == true)
		return _send_rpl(client, ERR_ALREADYREGISTRED, 462);
	client->set_password(tokens[1]);
	client->set_authentication(_password, client->get_password());
	if (client->get_authentication() == true)
		_send_welcome_msg(client);
	return SUCCESS;
}