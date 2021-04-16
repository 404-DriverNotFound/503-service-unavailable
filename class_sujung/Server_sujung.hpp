#pragma once
#include "Location_sujung.hpp"
#include "DataClass_sujung.hpp"

class Server : public ConfigSet
{
	private:
		std::string					server_name;
		u_int16_t					port;
		std::vector<Location>		location;
		Server();
		void	Server_setter(std::deque<std::string>&);
	public:
		Server(std::deque<std::string>&);
		~Server();
		// exception
		class InvalidConfig: public std::exception {
			public:
				virtual const char *what() const throw();
		};
};