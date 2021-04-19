#pragma once
#include "Location_sujung.hpp"
#include "DataClass_sujung.hpp"

struct Server : public ConfigSet
{
		std::string					server_name;
		u_int16_t					port;
		std::string					auth;
		std::vector<Location>		location;
		Server();
	private:
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