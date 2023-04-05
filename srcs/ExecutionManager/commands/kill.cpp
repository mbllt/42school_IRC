#include "ExecutionManager.hpp"

int ExecutionManager::kill(Client *client, token_vector tokens) {
	std::string	cmd("KILL");
	std::string	msg;
	Client		*malotru;

	if (client->get_fd() != _bot_fd)
		return _send_rpl(client, ERR_NOPRIVILEGES, 481);
	if (tokens.size() < 3)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	malotru = _client_book.find(_find_fd_client_by_name(tokens[1]))->second;
	msg = MSG_KILL(malotru->get_nickname(), tokens[2]);
	send(malotru->get_fd(), msg.c_str(), msg.size(), 0);
	malotru->leave_joined_channels("Malotru !!!", QUIT, _channel_book);
	close(malotru->get_fd());
	_client_book.erase(malotru->get_fd());
	delete malotru;
	return SUCCESS;
}