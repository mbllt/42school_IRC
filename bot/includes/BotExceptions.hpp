#ifndef BOTEXCEPTIONS_HPP
#define BOTEXCEPTIONS_HPP

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

namespace BotClientExceptions
{
	EXCEPTION(err_socket, "BotClient: Couldn't create socket and bind to the port.")
	EXCEPTION(err_addrinfo, "getaddrinfo error: ")
	EXCEPTION(err_listen, "BotClient: listen: ")
	EXCEPTION(err_poll, "BotClient: poll: ")
}

#endif