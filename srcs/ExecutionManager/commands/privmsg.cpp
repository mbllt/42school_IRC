#include "ExecutionManager.hpp"

typedef std::pair<std::vector<std::string>, std::vector<int> >			info_dest;

int	ExecutionManager::_err_privmsg_handling(Client *client, token_vector tokens, std::string cmd) {

	std::string msg;

	if (tokens.size() == 1)
		return _send_rpl(client, ERR_NORECIPIENT(cmd), 411);
	else if (tokens.size() == 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);
	if (client->get_authentication() == false)
		return _send_rpl(client,ERR_NOTREGISTERED, 451);
	return SUCCESS;
}

std::string	ExecutionManager::_assemble_msg(token_vector token_msg) {

	std::string msg;

	for (size_t i = 2; i < token_msg.size(); ++i) {
		msg.push_back(' ');
		msg.append(token_msg[i]);
	}
	return msg;
}

info_dest	ExecutionManager::_infos_dests(std::string str) {
	std::vector<std::string> str_dests = _split(str, ",");
	std::vector<int> dests_fd;
	for (size_t i = 0; i < str_dests.size(); ++i) {
		dests_fd.push_back(_find_fd_client_by_name(str_dests[i]));
	}
	return std::make_pair(str_dests, dests_fd);
}

bool	ExecutionManager::_dests_fd_valid(std::vector<int> dests) {
	for (size_t i = 0; i < dests.size(); ++i)
		if (!dests[i])
			return false;
	return true;
}

int	ExecutionManager::_msg_to_nicknames(Client *client, token_vector tokens, info_dest dests) {
	std::vector<std::string> str_dests = dests.first;
	std::vector<int> fd_dests = dests.second;
	std::string text = tokens[2];

	if (tokens.size() > 3) // in case no :
		text = _assemble_msg(tokens);

	for (size_t i = 0; i < str_dests.size(); ++i) {
		std::string msg = ((tokens[0] == "PRIVMSG") ? 	MSG_PRIVMSG(client->get_nickname(), str_dests[i], text) : \
														MSG_NOTICE(client->get_nickname(), str_dests[i], text));
		
		if (fd_dests[i] != _bot_fd)
			send(fd_dests[i], msg.c_str(), msg.size(), 0);
		if (_bot_fd != 0 && client->get_fd() != _bot_fd)
			send(_bot_fd, msg.c_str(), msg.size(), 0);
	}
	return SUCCESS;
}

int	ExecutionManager::_msg_to_channel(Client *client, token_vector tokens, Channel::iterator chan_it) {
	std::string dest = tokens[1];
	std::string text = tokens[2];
	Channel* chan = chan_it->second;

	if (tokens.size() > 3) // in case no :
		text = _assemble_msg(tokens);

	if (chan->user_is_in_channel(client) == false)
		_send_rpl(client, ERR_CANNOTSENDTOCHAN(chan_it->first), 404);
	std::string msg = ((tokens[0] == "PRIVMSG") ? 	MSG_PRIVMSG(client->get_nickname(), dest, text) : \
													MSG_NOTICE(client->get_nickname(), dest, text));
	if (_bot_fd != 0)
		send(_bot_fd, msg.c_str(), msg.size(), 0);
	chan->broadcast(client, msg);
	return SUCCESS;
}

int	ExecutionManager::privmsg(Client *client, token_vector tokens) {
	std::string cmd("PRIVMSG");
	int ret = _err_privmsg_handling(client, tokens, cmd);
	if (ret != SUCCESS)
		return ret;

	std::pair<std::vector<std::string>, std::vector<int> > dests = _infos_dests(tokens[1]);
	Channel::iterator chan_it = _find_chan_in_lowercase(tokens[1]);

	if (_dests_fd_valid(dests.second))
		ret = _msg_to_nicknames(client, tokens, dests);
	else if (chan_it != _channel_book.end()) {
		ret = _msg_to_channel(client, tokens, chan_it);
	}
	else
		_send_rpl(client, ERR_NOSUCHNICK(tokens[1]), 401);
	return ret;
}

// // ----ERR we are doing---
// // 411 ERR_NORECIPIENT ":No recipient given (<commande>)" = pas de destinataire
// // 401 ERR_NOSUCHNICK "<pseudonyme> :No such nick/channel"
// // 404 ERR_CANNOTSENDTOCHAN "<nom de canal> :Cannot send to channel" =
// 						Envoyé à un utilisateur qui (a) soit n'est pas dans
// 						un canal en mode +n ou (b) n'est pas opérateur (ou mode +v)
// 						sur un canal en mode +m ; et essaie d'envoyer un PRIVMSG à ce canal.
// 						!!! We don't have to do it because we don't do those flags (n, b, m, v)
// //-------------------------


// // ----ERR we are doing ????---
// // 412 ERR_NOTEXTTOSEND ":No text to send" = Pas de texte à envoyer
// 						// -> I could not replicate this err,
// 						// I only get : No enough param.
// //----------------------------


// // ----ERR we are NOT doing---
// // 	RPL_AWAY "<pseudo> :<message d'absence>"
// // 	ERR_NOTOPLEVEL "<masque> :No toplevel domain specified" = Domaine principal non spécifié.
// // 	ERR_WILDTOPLEVEL "<masque> :Wildcard in toplevel domain" = Joker dans le domaine principal
// // 	ERR_TOOMANYTARGETS "<destination> :Duplicate recipients. No message delivered" 
// 						// Renvoyé à un client qui essaie d'envoyer un PRIVMSG/NOTICE
// 						// utilisant le format de destination utilisateur@hôte pour lequel
// 						// utilisateur@hôte a plusieurs occurrences.
// //-------------------------
