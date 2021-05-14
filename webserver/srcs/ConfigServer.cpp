#include "../includes/ConfigServer.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Tokenizer.hpp"

//------------------------------------------------------------------------------
ConfigServer::ConfigServer()
{}

//------------------------------------------------------------------------------
ConfigServer::ConfigServer(map<string, string>& config_server_map)
{
	ft::atoi		(	config_server_map["port"]		);
	set_name		(	config_server_map["name"]		);
	set_root		(	config_server_map["root"]		);
	set_index_page	(	config_server_map["index_page"]	);
	set_error_page	(	config_server_map["error_page"]	);
	set_method		(	config_server_map["method"]		);
	set_head_max	(	config_server_map["head_max"]	);
	set_body_max	(	config_server_map["body_max"]	);
	set_autoindex	(	config_server_map["autoinedx"]	);
	set_timeout		(	config_server_map["timeout"]	);
}


void			ConfigServer::set_name(string& val)
{
	if (val.empty())
	{
		cout << "<name> ";
		throw ConfigElementEmpty();
	}
	_name.swap(val);
}

void			ConfigServer::set_root(string& val)
{
	if (val.empty())
	{
		cout << "<root> ";
		throw ConfigElementEmpty();
	}
	_root.swap(val);
}

void			ConfigServer::set_set(set<string>& page, string& val)
{

	Tokenizer	tokenizer(val);
	while (42)
	{
		string	token = tokenizer.chr(' ');
		if (token.empty())
			break;
		page.insert(token);
	}
}

void			ConfigServer::set_index_page(string& val)
{
	set_set(_index_page, val);
}

void			ConfigServer::set_error_page(string& val)
{
	set_set(_error_page, val);
}

void			ConfigServer::set_method(string& val)
{
	set_set(_method, val);
}

void			ConfigServer::set_head_max(string& val)
{
	if (val.empty())
	{
		_head_max = 0x2000;
		return;
	}
	_head_max = ft::atoi(val);
}

void			ConfigServer::set_body_max(string& val)
{
	if (val.empty())
	{
		_head_max = 0x20000;
		return;
	}
	_head_max = ft::atoi(val);
}

void			ConfigServer::set_autoindex(string& val)
{
	if (val.empty())
	{
		_autoindex = false;
		return;
	}
	if (val == "on")
		_autoindex = true;
	else if (val == "off")
		_autoindex = false;
	else
		throw ConfigElementInvalid();
	
}

void			ConfigServer::set_timeout(string& val)
{
	if (val.empty())
	{
		_timeout = 30000000;
		return ;
	}
	_timeout = ft::atoi(val);
	if (!_timeout)
		throw ConfigElementInvalid();
}

const char*		ConfigServer::ConfigElementEmpty::what() const throw()
{
	return "Element not found.";
}

const char*		ConfigServer::ConfigElementInvalid::what() const throw()
{
	return "Invalid elemnet.";
}