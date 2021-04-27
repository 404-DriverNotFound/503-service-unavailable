#pragma once
#include "Location.hpp"
#include "DataClass.hpp"
#include "Socket.hpp"
#include <map>

/*##############################################################################
Server
##############################################################################*/

using std::string;
using std::map;

struct Server : public ConfigSet
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	uint16_t				port;
	map<string, Location>	locations;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	Server();
	public:
				Server(std::deque<std::string>&);
				~Server();

	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void		Server_setter(std::deque<std::string>&);

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