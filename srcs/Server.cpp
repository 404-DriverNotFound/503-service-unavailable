#include <iostream>
#include "Server.hpp"
#include "Utils.hpp"

//------------------------------------------------------------------------------

Server::Server(std::deque<std::string>& token)
{
	Server_setter(token);
}

//------------------------------------------------------------------------------

Server::~Server()
{
}

//------------------------------------------------------------------------------

void	Server::Server_setter(std::deque<std::string>&	token)
{
	std::string	str = " \t";
	char	*base, *seq = str.begin().base();

	base = token[0].begin().base();
	while (!strncmp(base, "\t", 1))
	{
		++base;
		bool	flag = true;
		if (!strncmp(base, "location", 8))
		{
			location.push_back(Location(token));
			flag = false;
		}
		else if (!strncmp(base, "root", 4))
		{
			root += (base + 5);
		}
		else if (!strncmp(base, "server_name", 11))
		{
			server_name += (base + 12);
		}
		else if (!strncmp(base, "port", 4))
		{
			port = static_cast<u_int16_t>(atoi(base + 4));
		}
		else if (!strncmp(base, "error_page", 10))
		{
			std::string::iterator	it;
			std::string				temp;

			it = token[0].begin() + 12;
			while (get_set_token(token[0], it, temp, seq))
			{
				error_page.push_back(temp);
			}
		}
		else if (!strncmp(base, "index", 5))
		{
			std::string::iterator	it;
			std::string				temp;

			it = token[0].begin() + 7;
			while (get_set_token(token[0], it, temp, seq))
			{
				index.push_back(temp);
			}
			index.push_back(temp);
		}
		else if (!strncmp(base, "head_length", 11))
		{
			head_length = static_cast<u_int32_t>(atoi(base + 12));
		}
		else if (!strncmp(base, "body_length", 11))
		{
			body_length = static_cast<u_int16_t>(atoi(base + 12));
		}
		else if (!strncmp(base, "autoindex", 8))
		{
			if (!strncmp(base + 9, " on", 0))
				autoindex = true;
			else
				autoindex = false;
		}
		else if (!strncmp(base, "timeout", 7))
		{
				timeout = static_cast<u_int16_t>(atoi(base + 7));
		}
		else if (!strncmp(base, "auth", 4))
		{
			auth += base + 5;
		}
		else
			throw Server::InvalidConfig();
		if (flag)
			token.pop_front();
		base = token[0].begin().base();
	}
}

/*--------------------------------------------------------------------------
Exception
--------------------------------------------------------------------------*/

const char*	Server::InvalidConfig::what() const throw()
{
	return ("Server Config is Invalid");
} 