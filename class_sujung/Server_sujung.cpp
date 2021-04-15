#include <iostream>
#include "Server_sujung.hpp"

Server::Server(std::deque<std::string>& token)
{
	Server_setter(token);
}

Server::~Server()
{
}

void	Server::Server_setter(std::deque<std::string>&	token)
{
	char					*base;

	base = token[0].begin().base();
	while (!strncmp(base, "\t", 0))
	{
		if (strncmp(base, "location", 1))
		{
			token.pop_front();
			location.push_back(Location(token));
		}
		else if (strncmp(base, "root", 1))
		{

		}
		else if (strncmp(base, "server_name", 1))
		{

		}
		else if (strncmp(base, "port", 1))
		{

		}
		else if (strncmp(base, "error_page", 1))
		{

		}
		else if (strncmp(base, "index", 1))
		{

		}
		else if (strncmp(base, "head_length", 1))
		{

		}
		else if (strncmp(base, "body_length", 1))
		{

		}
		else if (strncmp(base, "autoindex", 1))
		{

		}
		else if (strncmp(base, "timeout", 1))
		{

		}
		else if (strncmp(base, "auth", 1))
		{

		}
		else
			throw Server::InvalidConfig();
		token.pop_front();
		base = token[0].begin().base();
	}
}