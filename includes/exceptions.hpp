/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 15:21:27 by llecoq            #+#    #+#             */
/*   Updated: 2022/09/07 14:20:06 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>


#define	EXCEPTION(name, err_msg)												\
class name : public std::exception												\
{																				\
	private:																	\
		std::string	_err_msg;													\
	public:																		\
		name() : _err_msg(err_msg){}											\
		name(std::string err_infos) : _err_msg(err_msg + err_infos) {}			\
		virtual ~name() throw() {return ;}										\
																				\
		virtual const char * what() const throw()								\
		{																		\
			return _err_msg.c_str();											\
		}																		\
};

namespace ServerExceptions
{
	EXCEPTION(err_socket, "Server: Couldn't create socket and bind to the port.")
	EXCEPTION(err_addrinfo, "getaddrinfo error: ")
	EXCEPTION(err_listen, "Server: listen: ")
	EXCEPTION(err_poll, "Server: poll: ")
}

#endif