#include "ExecutionManager.hpp"

#define REMOVE '-'
#define ADD '+'

int ExecutionManager::_display_infos_mode(Client *client, token_vector tokens, Channel* chan) {

	Channel::iterator chan_it = _find_chan_in_lowercase(tokens[1]);

	if (chan_it == _channel_book.end())
		return _send_rpl(client, ERR_NOSUCHCHANNEL(tokens[1]), 403);

	return _send_rpl(client, RPL_CHANNELMODEIS(client->get_nickname(), chan->get_name(), chan->get_modes(), std::string()), 324);
}

int	ExecutionManager::_err_mode_handling(Client *client, token_vector tokens,\
											Channel::iterator chan_it, Channel* chan) {
	std::string cmd = "MODE";

	if (tokens.size() < 2)
		return _send_rpl(client, ERR_NEEDMOREPARAMS(cmd), 461);

	if (chan_it == _channel_book.end())
		return _send_rpl(client, ERR_NOSUCHCHANNEL(tokens[1]), 403);
	else if (chan->user_is_in_channel(client) == false)
		return _send_rpl(client, ERR_NOTONCHANNEL(tokens[1]), 403);
	else if (client != chan->get_operator())
		return _send_rpl(client, ERR_CHANOPRIVSNEEDED(tokens[1]), 482);

	// parsing
	std::string valid_modes("+-it");
	for (size_t i = 0; i < tokens[2].size(); ++i) {
		// SEGFAULT
		if (valid_modes.find(tokens[2][i]) == std::string::npos) {
			return _send_rpl(client, ERR_UNKNOWNMODE((tokens[2].substr(i, 1))), 472);
		}
	}
	return SUCCESS;
}

bool	ExecutionManager::_is_add_rmv(char c) {
	if (c == ADD || c == REMOVE)
		return true;
	return false;
}

std::string	ExecutionManager::_add_flags(Channel* chan, std::string new_flags) {

	std::string set;
	size_t start = 0;
	size_t end = 0;

	while ((start = new_flags.find_first_not_of(ADD, end)) != std::string::npos) {
		end = new_flags.find(ADD, start);
		for (size_t i = start; (i < new_flags.size() && !_is_add_rmv(new_flags[i])); ++i) {
			char to_add = new_flags[i];
			if ((chan->get_modes()).find(to_add) == std::string::npos) {
				chan->set_modes(to_add, 0, ADD);
				if (set.empty())
					set.append(1, ADD);
				set.append(1, to_add);
			}
		}
	}
	return set;
}

std::string ExecutionManager::_remove_flags(Channel* chan, std::string new_flags) {

	std::string set;
	size_t start = 0;
	size_t end = new_flags.find_first_of(REMOVE, start);

	while ((start = new_flags.find_first_not_of(REMOVE, end)) != std::string::npos) {
		end = new_flags.find(REMOVE, start);
		for (size_t i = start; (i < new_flags.size() && !_is_add_rmv(new_flags[i])); ++i) {
			char to_rmv = new_flags[i];
			size_t pos_to_rmv = (chan->get_modes()).find(to_rmv);
			if (pos_to_rmv != std::string::npos) {
				chan->set_modes(to_rmv, pos_to_rmv, REMOVE);
				if (set.empty())
					set.append(1, REMOVE);
				set.append(1, to_rmv);
			}
		}
	}
	return set;
}

int	ExecutionManager::mode(Client *client, token_vector tokens) {

	Channel::iterator chan_it = _find_chan_in_lowercase(tokens[1]);
	Channel* chan = chan_it->second;

	if (tokens.size() == 2)
		return _display_infos_mode(client, tokens, chan);

	int ret = _err_mode_handling(client, tokens, chan_it, chan);
	if (ret != SUCCESS)
		return ret;

	std::string ret_flags = _add_flags(chan, tokens[2]);
	ret_flags += _remove_flags(chan, tokens[2]);

	if (!ret_flags.empty())
		return _send_rpl(client, RPL_CHANNELMODEIS(client->get_nickname(), chan->get_name(), ret_flags, std::string()), 324);
	return SUCCESS;
}
// return SUCCESS if no flag was removed or added


// // RFC 1459
// ------ USER MODES <nickname> {+/- o|i|w|s}
//		-o = operator flag
//		-i = marks a users as invisible
//		-w = user recevives wallops. Wallopas
//				are messages that can broadcast
//				network information and its
//				status to its users.
//		-s = marks a user as invisible
// //----------------------------

// ------ CHANNEL MODES <channel> {+/- o|p|s|i|t|n||m|l|b|v|k} [limit] [user] [ban mask]
//		-o = give/take channel operator privileges
//		-p = private channel flag
//		-s = secret channel flag
//		-i = invite-only channel flag								V
//		-t = topic settable by channel operator only flag			V
//		-n = no messages to channel from clients on the outside
//		-m = moderated channel
//		-l = set the user limit to channel
//		-b = set a ban mask to keep users out
//		-v = give/take the ability to speak on a moderated channel
//		-k = set a channel key (password)
// //--------------------------------------------------------



// // ERR
// // ----ERR we are doing---
//		461 ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
//		403 ERR_NOSUCHCHANNEL "<channel name> :No such channel"
//		442 ERR_NOTONCHANNEL "<channel> :You're not on that channel"
//		482 ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
//		472 ERR_UNKNOWNMODE "<char> :is unknown mode char to me"
// //----------------------------

// // ----ERR we are doing ????---
//	ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag" =
//							Returned by the server to indicate
//							that a MODE message was sent with a
//							nickname parameter and that the a
//							mode flag sent was not recognized.
// //----------------------------

// // ----ERR we are NOT doing---
//	ERR_USERSDONTMATCH ":Cant change mode for other users" =
//						Error sent to any user trying to view
//						or change the user mode for a user other
//						than themselves.
//	ERR_KEYSET "<channel> :Channel key already set"
//	ERR_NOSUCHNICK "<nickname> :No such nick/channel"
// //--------------------------------------------------------



// // RPL
// // ----RPL we are doing---
//	324 RPL_CHANNELMODEIS "<channel> <mode> <mode params>"
// //----------------------------

// // ----RPL we are doing ????---
//	221 RPL_UMODEIS "<user mode string>"
// //----------------------------

// // ----RPL we are NOT doing---
//	RPL_BANLIST "<channel> <banid>"
//	RPL_ENDOFBANLIST "<channel> :End of channel ban list"
// //--------------------------------------------------------