/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotClient.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 20:28:03 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/14 12:17:34 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTCLIENT_HPP
# define BOTCLIENT_HPP

# include "BotCommons.hpp"
# include "BotExceptions.hpp"
# include "Bot.hpp"

#define NO_TIMEOUT -1

enum	e_events
{
	CONNECTION_LOST,
	DATA_RECEIVED
};

class BotClient
{

	public:

		BotClient(const char *port);
		~BotClient();

		void			init();
		void			run();

	private:

		BotClient();

		void			_get_address_info();
		void			_get_receiving_socket();
		int				_connect_to_socket(addrinfo *ptr);
		void			_poll_events();
		int				_find_event();
		static void		_signal_handler(int signum);

		const char		*_port;
		struct addrinfo	*_servinfo;
		struct pollfd	*_pollfd;
		int				_receiving_socket;
		Bot				_bot;
};

#endif /* ************************************************************* Client_H */