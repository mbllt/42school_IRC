/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 20:28:08 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/14 09:58:29 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BotClient.hpp"

static volatile bool	bot_running = true;

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

BotClient::BotClient() {}

BotClient::BotClient(const char* port) : _port(port), _pollfd(new pollfd) {}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

BotClient::~BotClient() 
{
	delete _pollfd;
	close(_receiving_socket);
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	BotClient::init()
{
	_get_address_info();
	_get_receiving_socket();
	_bot.init_bot(_receiving_socket);
}

void	BotClient::run()
{
	std::cout << "Bot is now conneted to server !" << std::endl;
	while (bot_running == true)
	{
		_poll_events();
		switch (_find_event())
		{
		case DATA_RECEIVED:
			std::cout << "DATA RECEIVED" << std::endl;
			_bot.process_data();
			break;
		case CONNECTION_LOST:
			std::cout << "CONNECTION_LOST" << std::endl;
			bot_running = false;
			break;
		}
	}
}

void	BotClient::_signal_handler(int signum)
{
	if (signum == SIGINT)
		bot_running = false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */