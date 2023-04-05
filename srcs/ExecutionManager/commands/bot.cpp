#include "ExecutionManager.hpp"

int ExecutionManager::bot(Client *client, token_vector tokens) {
	if (tokens.size() != 3)
		return FAILED;
	if (tokens[1] == "bot" || tokens[2] == ":motdepassecomplique") {
		std::string	bot("bot");

		_bot_fd = client->get_fd();
		client->set_username("Claptrap");
		client->set_nickname("bot");
		client->set_password("AUTHENTICATION");
		client->set_authentication("AUTHENTICATION", "AUTHENTICATION");
		_send_rpl(client, "Prouvez que vous êtes un robot pour continuer...\n", 001);
	}
	return SUCCESS;
}