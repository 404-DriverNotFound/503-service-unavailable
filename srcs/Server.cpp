#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"

//------------------------------------------------------------------------------

Server::Server(std::deque<std::string>& token)
: ConfigSet(), port(0)
{
	Server_setter(token);
}

Server::Server(const Server& x)
: ConfigSet(x),
port(x.port),
locations(x.locations)
{}

//------------------------------------------------------------------------------

Server::~Server()
{}

//------------------------------------------------------------------------------

void	Server::Server_setter(std::deque<std::string>&	lines)
{

	while (!lines.empty())
	{
		int		indent = ft::count_chr(lines.front(), '\t');
		lines.front().erase(lines.front().begin(), lines.front().begin() + indent);
		list<string>	tokens = ft::get_set_token(lines.front(), " ");
		string&			key = tokens.front();
		if (key == "server")
		{
			break;
		}
		if (tokens.size() < 2)
			throw ConfigSet::InvalidConfig();
		string&			val = *++tokens.begin();

		if (key == "location")
		{
			string		name = val;
			lines.pop_front();
			Location	tmp(lines);
			locations.insert(std::make_pair(name, tmp));
			if (locations[name].root == "/")
				locations[name].root = root;
			else
			{
				string		location_root;
				location_root = root + locations[name].root;
				locations[name].root = location_root;
			}
			if (lines.empty())
				break;
			continue;
		}
		else if (key == "root")
		{
			root = val;
		}
		else if (key == "server_name")
		{
			name = val;
		}
		else if (key == "port")
		{
			port = ft::atoi(val);
		}
		else if (key == "error_page")
		{
			for (list<string>::iterator it = ++tokens.begin() ; it != tokens.end() ; ++it)
			{
				error_page.insert(*it);
			}
		}
		else if (key == "index")
		{
			for (list<string>::iterator it = ++tokens.begin() ; it != tokens.end() ; ++it)
			{
				index.insert(*it);
			}
		}
		else if (key == "head_length")
		{
			head_length = ft::atoi(val);
		}
		else if (key == "body_length")
		{
			body_length = ft::atoi(val);
		}
		else if (key == "autoindex")
		{
			if (val == "on")
				autoindex = true;
			else if (val == "off")
				autoindex = false;
			else
				throw ConfigSet::InvalidConfig();
		}
		else if (key == "timeout")
		{
			timeout = ft::atoi(val);
		}
		else
		{
			throw ConfigSet::InvalidConfig();
		}
		lines.pop_front();
	}
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
		os << "off" << std::endl;
	for (map<string, Location>::iterator it = ref.locations.begin() ; it != ref.locations.end() ; ++it)
		os << it->first << " : " << it->second << std::endl;
	return os;
}
