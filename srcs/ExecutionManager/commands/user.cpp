#include "ExecutionManager.hpp"

static bool	username_is_not_valid(std::string username);

int ExecutionManager::user(Client *client, token_vector tokens) {
	std::string	cmd("USER");

	if (tokens.size() < 5)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (client->get_username().empty() == false)
		return _send_rpl(client, ERR_ALREADYREGISTRED, 462);
	if (username_is_not_valid(tokens[1]) == true)
		return _send_rpl(client, ERR_INVALIDUSERNAME, 468);
	client->set_username(tokens[1]);
	client->set_realname(tokens[4]);
	client->set_authentication(_password, client->get_password());
	if (client->get_authentication() == true)
		_send_welcome_msg(client);
	return SUCCESS;
}

static bool	username_is_not_valid(std::string username) {
	for (size_t i = 0; i < username.size(); i++)
		if (isalnum(username[i]) == false)
			return true;
	return false;	
}