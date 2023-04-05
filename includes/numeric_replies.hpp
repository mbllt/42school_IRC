/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_replies.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnel <abonnel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 13:34:23 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/14 13:35:22 by abonnel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERIC_REPLIES_HPP
#define NUMERIC_REPLIES_HPP

#define CRLF 											"\r\n"
#define SERVER_NAME										"ft_irc"
#define SERVER_VERSION									"1.0"

// ERROR REPLIES 1459
#define ERR_NOSUCHNICK(nickname)						"401 " + nickname + " :No such nick/channel" + CRLF
#define ERR_NOSUCHCHANNEL(channel)						"403 " + channel + " :No such channel" + CRLF
#define ERR_UNKNOWNCOMMAND(command)						"421 " + command + " :Unknown command" + CRLF
#define ERR_NONICKNAMEGIVEN								"431 :No nickname given\n\r"
#define ERR_ERRONEUSNICKNAME(nickname)					"432 " + nickname + " :Erroneous nickname" + CRLF		
#define ERR_NICKNAMEINUSE(nickname)						"433 " + nickname + " :Nickname is already in use" + CRLF
#define ERR_NOTONCHANNEL(channel)						"442 " + channel + " :You're not on that channel" + CRLF
#define ERR_INVALIDUSERNAME								"468 * USER :Your username is not valid\n\r"
#define ERR_UNKNOWNMODE(char)							"472 " + char + " :is unknown mode char to me" + CRLF
#define ERR_CHANOPRIVSNEEDED(channel)					"482 " + channel + " :You're not channel operator" + CRLF

// ERROR REPLIES 2812
#define ERR_NOSUCHCHANNEL(channel)						"403 " + channel + " :No such channel" + CRLF
#define ERR_CANNOTSENDTOCHAN(channel)					"404 " + channel + " :Cannot send to channel" + CRLF
#define ERR_NORECIPIENT(command)						"411 :No recipient given " + command + CRLF
#define ERR_USERNOTINCHANNEL(nickname, channel)			"441 " + nickname + " " + channel + " :They aren't on that channel" + CRLF
#define ERR_NOTONCHANNEL(channel)						"442 " + channel + " :You're not on that channel" + CRLF
#define ERR_USERONCHANNEL(nickname, channel)			"443 " + nickname + " " + channel + " :is already on channel" + CRLF
#define ERR_NOTREGISTERED								"451 :You have not registrered\r\n"
#define ERR_NEEDMOREPARAMS(command)						"461 " + command + " :Not enough parameters" + CRLF
#define ERR_ALREADYREGISTRED							"462 :Unauthorized command (already registered)\r\n"
#define ERR_INVITEONLYCHAN(channel)						"473 " + channel + " :Cannot join channel (+i)" + CRLF
#define ERR_BADCHANNAME(channel)						"479 " + channel + " :Invalid channel name" + CRLF
#define ERR_NOPRIVILEGES								"481 :Permission Denied- You're not an IRC operator\r\n"

// COMMAND RESPONSES RFC_2812
#define RPL_WELCOME(nickname)							"001 " + nickname + " :Welcome to the forbidden ft_irc serveur " + nickname + CRLF
#define RPL_YOURHOST(nickname)							"002 " + nickname + " :Your host is ft_irc, running version 1.0" + CRLF
#define RPL_LUSERCLIENT(nickname, number_of_clients)	"251 " + nickname + " :There are " + number_of_clients +  " users on 1 servers" + CRLF
#define RPL_LUSERCHANNELS(nickname, number_of_channels)	"254 " + nickname + " :There are " + number_of_channels +  " channels formed" + CRLF
#define RPL_CHANNELMODEIS(nickname, channel, mode, mode_params)	"324 " + nickname + " " + channel + " :" + mode + mode_params + CRLF
#define RPL_NOTOPIC(nickname, channel) 					"331 " + nickname + " " + channel + " :No topic is set" + CRLF
#define RPL_TOPIC(nickname, channel, topic) 			"332 " + nickname + " " + channel + " :" + topic + CRLF
#define RPL_NAMREPLY(channel, nickname, members)		"353 " + nickname + " " + channel + " :" + members + CRLF
#define RPL_INVITING(inviting, channel, invited)		"341 " + inviting + " " + invited + " " + channel + CRLF
#define RPL_ENDOFNAMES(channel, nickname)				"366 " + nickname + " " + channel + " :End of /NAMES list" + CRLF
#define RPL_MOTD(nickname, message)						"372 " + nickname + " :" + message + CRLF
#define RPL_ENDOFMOTD(nickname)							"376 " + nickname + " :End of message of the day." + CRLF

// MSG_BUILDER
#define MSG_JOIN(channel, nickname)						":" + nickname + " JOIN " + channel + CRLF
#define MSG_PART(channel, nickname, part_msg)			":" + nickname + " PART " + channel +  " " + part_msg + CRLF
#define MSG_KICK(operator, channel, kicked, kick_msg)	":" + operator + " KICK " + channel +  " " + kicked + " " + kick_msg + CRLF
#define MSG_INVITE(inviting, invited, channel)			":" + inviting + " INVITE " + invited +  " " + channel + CRLF
#define MSG_NICK(old_nickname, new_nickname)			":" + old_nickname + " NICK " + new_nickname + CRLF
#define MSG_MODE(nickname)								":" + nickname + " MODE " + nickname + " :+w" + CRLF
#define MSG_KILL(nickname, message)						":bot KILL " + nickname + " " + message + CRLF
#define MSG_PRIVMSG(sender, recipient, msg)				":" + sender + " PRIVMSG " + recipient + " " + msg + CRLF
#define MSG_NOTICE(sender, recipient, msg)				":" + sender + " NOTICE " + recipient + " " + msg + CRLF

// #define MSG_NOTICE(rpl, sending_nickname, recipient, msg)	rpl + ":" + sending_nickname + " NOTICE " + recipient + " :" + msg + CRLF


#endif // NUMERIC_REPLIES_HPP