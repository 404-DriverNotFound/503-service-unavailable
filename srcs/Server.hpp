#pragma once
#include "Location.hpp"
#include "DataClass.hpp"
#include "Socket.hpp"

/*##############################################################################
Http
##############################################################################*/
struct Server : public ConfigSet
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	Socket						sock;
	std::string					server_name;
	uint16_t					port;
	std::string					auth;
	std::vector<Location>		location;
	Server();

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
				Server(std::deque<std::string>&);
				~Server();

	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void	Server_setter(std::deque<std::string>&);

	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	class InvalidConfig: public std::exception {
		public:
			virtual const char *what() const throw();
	};
	class SelectFailed: public std::exception {
		public:
			virtual const char *what() const throw();
	};
};

std::ostream&	operator<<(std::ostream&, Server&);
