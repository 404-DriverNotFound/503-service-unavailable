#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include "Server_sujung.hpp"

struct WebServ
{
		std::vector<Server>		server;
	private:
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

std::ostream&	operator<<(std::ostream&, WebServ&);