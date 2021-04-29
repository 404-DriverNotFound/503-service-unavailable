#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"

//------------------------------------------------------------------------------

Server::Server(std::deque<std::string>& token)
{
	Server_setter(token);
}

//------------------------------------------------------------------------------

Server::~Server()
{}

//------------------------------------------------------------------------------

void	Server::Server_setter(std::deque<std::string>&	token)
{
	std::string	str = " \t";
	char	*base, *seq = str.begin().base();

	base = token[0].begin().base();
	while (!token.empty() && !ft::strncmp(base, "\t", 1))
	{
		++base;
		bool	flag = true;
		if (!ft::strncmp(base, "location", 8))
		{
			Location	temp(token);
			std::cout << temp.name << std::endl;
			locations.insert(std::make_pair(temp.name, temp));
			// std::cout << "location size : " << locations.size() << std::endl;
			flag = false;
		}
		else if (!ft::strncmp(base, "root", 4))
		{
			root += (base + 5);
		}
		else if (!ft::strncmp(base, "server_name", 11))
		{
			name += (base + 12);
		}
		else if (!ft::strncmp(base, "port", 4))
		{
			port = static_cast<u_int16_t>(atoi(base + 5));
		}
		else if (!ft::strncmp(base, "error_page", 10))
		{
			std::string::const_iterator	it;
			std::string					temp;

			it = token[0].begin() + 12;
			while (ft::get_set_token(token[0], it, temp, seq))
			{
				error_page.insert(temp);
			}
			error_page.insert(temp);
		}
		else if (!ft::strncmp(base, "index", 5))
		{
			std::string::const_iterator	it;
			std::string					temp;

			it = token[0].begin() + 7;
			while (ft::get_set_token(token[0], it, temp, seq))
			{
				index.insert(temp);
			}
			index.insert(temp);
		}
		else if (!ft::strncmp(base, "head_length", 11))
		{
			head_length = static_cast<u_int32_t>(atoi(base + 12));
		}
		else if (!ft::strncmp(base, "body_length", 11))
		{
			body_length = static_cast<u_int64_t>(atoi(base + 12));
		}
		else if (!ft::strncmp(base, "autoindex", 8))
		{
			if (!ft::strncmp(base + 9, " on", 0))
				autoindex = true;
			else
				autoindex = false;
		}
		else if (!ft::strncmp(base, "timeout", 7))
		{
			timeout = static_cast<u_int16_t>(atoi(base + 7));
		}
		else
			throw Server::InvalidConfig();
		if (flag)
			token.pop_front();
		base = token[0].begin().base();
	}
	// Location	temp(*this);
	// locations.insert(std::make_pair("/", temp));
}

/*--------------------------------------------------------------------------
Exception
--------------------------------------------------------------------------*/

const char*	Server::InvalidConfig::what() const throw()
{
	return ("Server Config is Invalid");
} 

std::ostream&	operator<<(std::ostream& os, Server& ref) {
	os << "	root: " << ref.root << std::endl
		<<"	server_name: " << ref.name << std::endl
		<<"	error_page: ";
	std::set<std::string>::iterator	it;
	it = ref.error_page.begin();
	for (;it != ref.error_page.end();++it)
		os << *it << " ";
	os << std::endl
		<< "	port: " << ref.port << std::endl
		<< "	index: ";
	it = ref.index.begin();
	for (; it != ref.index.end();++it)
		os << *it << " ";
	os << std::endl
		<< "	head_length: " << ref.head_length << std::endl
		<< "	body_length: " << ref.body_length << std::endl
		<< "	autoindex: ";
	if (ref.autoindex)
		os << "on";
	else
		os << "off";
	os << std::endl
		<< "	auth: " << ref.auth << std::endl;
	for (map<string, Location>::iterator it = ref.locations.begin() ; it != ref.locations.end() ; ++it)
		os << it->first << " : " << it->second << std::endl;
	return os;
}
