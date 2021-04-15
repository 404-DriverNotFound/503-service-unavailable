#pragma once
#include <vector>
#include <deque>
#include "Server_sujung.hpp"

class WebServ
{
	private:
		std::vector<Server>		server;
		void	config_parser(std::deque<std::string>&);
		void	server_create(std::deque<std::string>&);
	public:
		WebServ();
		~WebServ();
		// exception
		class InvalidServerBlock: public std::exception {
            public:
                virtual const char *what() const throw();
        };
};