#include "ExecutionManager.hpp"

static bool	nickname_is_valid(std::string nickname);

int ExecutionManager::nick(Client *client, token_vector tokens) {
	std::string	cmd("NICK");

	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NONICKNAMEGIVEN, 431);

	std::string	nickname = tokens[1];
	if (nickname_is_valid(nickname) == true) {
		if (_find_fd_client_by_name(nickname) > 0)
			return _send_rpl(client, ERR_NICKNAMEINUSE(nickname), 433);
		std::string	old_nickname = client->get_nickname();
		client->set_nickname(tokens[1]);
		if (client->get_authentication() == false) {
			client->set_authentication(_password, client->get_password());
			if (client->get_authentication() == true)
				return _send_welcome_msg(client);
		}
		else
			client->announce_new_nickname(MSG_NICK(old_nickname, tokens[1]));
		return SUCCESS;
	} // nickname is not valid
	return _send_rpl(client, ERR_ERRONEUSNICKNAME(nickname), 432);
}

static bool	nickname_is_valid(std::string nickname) {
	if (nickname.size() > 9 || isalpha(nickname[0]) == false)
		return false;
	if ((nickname.find('\0') != std::string::npos) || (nickname.find(' ') != std::string::npos)
		|| (nickname.find('\n') != std::string::npos) || (nickname.find('@') != std::string::npos)
		|| (nickname.find('\r') != std::string::npos))
		return false;
	return true;
}
