#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Webserver.hpp"
/*#####################################
Webserver
######################################*/
Webserver::Webserver(int argc, char** argv, char** env)
: select_timeout(5000000)
{
	if (argc > 2)
	{
		std::cout << "Usage: " << argv[0] << " [configfile]" << std::endl;
		return ;
	}
	std::deque<std::string>	token(1);
	config_parser(token, argv[1]);
	server_create(token);
	set_path_cgi_bin(env);
	set_status_code();
	set_map_method();
}

Webserver::~Webserver()
{}
//------------------------------------------------------------------------------

void	Webserver::config_parser(std::deque<std::string>& token, const char* config_path)
{
	char						buf[30];
	int							fd, n, idx, vdx = 0;


	if (!config_path)
	{
		config_path = "./server_config";
	}
	if ((fd = open(config_path, O_RDONLY)) < 0)
	{
		std::cout << "Config File Open Failed" << std::endl;
		return ;
	}
	while ((n = read(fd, buf, 29)) > 0)
	{
		buf[n] = 0;
		for (idx=0;idx<n;++idx)
		{
			if (buf[idx] == '\n')
			{
				token.push_back(std::string());
				++vdx;
			}
			else
				token[vdx].push_back(buf[idx]);
		}
	}
	close(fd);
}

//------------------------------------------------------------------------------

void	Webserver::server_create(std::deque<std::string>& token)
{
	std::string::iterator	it;


	while (!token.empty())
	{
		it = token[0].begin();
		if (!token.empty() && !ft::strncmp(it.base(), "server", 6) && token[0].length() == 6)
		{
			token.pop_front();
			// std::cout << "here\n";
			servers.push_back(Server(token));
		}
		else
			throw Webserver::InvalidServerBlock();
	}
}

//------------------------------------------------------------------------------

void			Webserver::start_server()
{
	int		result;

	while (42)
	{
		to_be_checked_read = to_be_checked;
		to_be_checked_write = to_be_checked;
		to_be_checked_exception = to_be_checked;
		result = select(max_connection, &to_be_checked_read, &to_be_checked_write, &to_be_checked_exception, &select_timeout);
		if (result < 0)
			throw SelectFailed();
		if (result == 0)	// timeout
			continue;
		check_new_connection();
		manage_clients();
	}
}

//------------------------------------------------------------------------------

void			Webserver::check_new_connection()
{
	server_iterator		end = servers.end();
	for (server_iterator it = servers.begin() ; it != end ; ++it)
	{
		if (to_be_checked_read.get(it->sock.fd) == 0)
			continue;
		clients.push_back(Client(it->sock.fd, servers));
	}
}

//------------------------------------------------------------------------------

void			Webserver::manage_clients()
{
	client_iterator		end = clients.end();
	for (client_iterator it = clients.begin() ; it != end ; ++it)
	{
		it->client_process(to_be_checked_read, to_be_checked_write);
		if (it->status == SEND_DONE)
			clients.erase(it);
	}
}

//------------------------------------------------------------------------------

const char*	Webserver::InvalidServerBlock::what() const throw()
{
	return ("Server Block start line name Invalid");
}

//------------------------------------------------------------------------------

const char*	Webserver::SelectFailed::what() const throw()
{
	return ("Select Failed");
}

//------------------------------------------------------------------------------

void		Webserver::set_path_cgi_bin(char** env)
{
	Cgi::cgi_bin["php"] = ft::which("php", env);
	Cgi::cgi_bin["py"] = ft::which("python3", env);
}

//------------------------------------------------------------------------------

void	Webserver::set_map_method()
{
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("CONNECT", CONNECT));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("DELETE", DELETE));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("GET", GET));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("HEAD", HEAD));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("OPTIONS", OPTIONS));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("PATCH", PATCH));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("POST", POST));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("PUT", PUT));
	Http::mapMethod.insert(std::make_pair<std::string, u_int16_t>("TRACE", TRACE));
}
//------------------------------------------------------------------------------

void		Webserver::set_status_code()
{
	HttpRes::status_code_map[100] = "Continue";
	HttpRes::status_code_map[101] = "Switching Protocols";
	HttpRes::status_code_map[103] = "Early Hints";
	HttpRes::status_code_map[200] = "OK";
	HttpRes::status_code_map[201] = "Created";
	HttpRes::status_code_map[202] = "Accepted";
	HttpRes::status_code_map[203] = "Non-Authoritative Information";
	HttpRes::status_code_map[204] = "No Content";
	HttpRes::status_code_map[205] = "Reset Content";
	HttpRes::status_code_map[206] = "Partial Content";
	HttpRes::status_code_map[300] = "Multiple Choices";
	HttpRes::status_code_map[301] = "Moved Permanently";
	HttpRes::status_code_map[302] = "Found";
	HttpRes::status_code_map[303] = "See Other";
	HttpRes::status_code_map[304] = "Not Modified";
	HttpRes::status_code_map[307] = "Temporary Redirect";
	HttpRes::status_code_map[308] = "Permanent Redirect";
	HttpRes::status_code_map[400] = "Bad Request";
	HttpRes::status_code_map[401] = "Unauthorized";
	HttpRes::status_code_map[402] = "Payment Required";
	HttpRes::status_code_map[403] = "Forbidden";
	HttpRes::status_code_map[404] = "Not Found";
	HttpRes::status_code_map[405] = "Method Not Allowed";
	HttpRes::status_code_map[406] = "Not Acceptable";
	HttpRes::status_code_map[407] = "Proxy Authentication Required";
	HttpRes::status_code_map[408] = "Request Timeout";
	HttpRes::status_code_map[409] = "Conflict";
	HttpRes::status_code_map[410] = "Gone";
	HttpRes::status_code_map[411] = "Length Required";
	HttpRes::status_code_map[412] = "Precondition Failed";
	HttpRes::status_code_map[413] = "Payload Too Large";
	HttpRes::status_code_map[414] = "URI Too Long";
	HttpRes::status_code_map[415] = "Unsupported Media Type";
	HttpRes::status_code_map[416] = "Range Not Satisfiable";
	HttpRes::status_code_map[417] = "Expectation Failed";
	HttpRes::status_code_map[418] = "I'm a teapot";
	HttpRes::status_code_map[422] = "Unprocessable Entity";
	HttpRes::status_code_map[425] = "Too Early";
	HttpRes::status_code_map[426] = "Upgrade Required";
	HttpRes::status_code_map[428] = "Precondition Required";
	HttpRes::status_code_map[429] = "Too Many Requests";
	HttpRes::status_code_map[431] = "Request Header Fields Too Large";
	HttpRes::status_code_map[451] = "Unavailable For Legal Reasons";
	HttpRes::status_code_map[500] = "Internal Server Error";
	HttpRes::status_code_map[501] = "Not Implemented";
	HttpRes::status_code_map[502] = "Bad Gateway";
	HttpRes::status_code_map[503] = "Service Unavailable";
	HttpRes::status_code_map[504] = "Gateway Timeout";
	HttpRes::status_code_map[505] = "HTTP Version Not Supported";
	HttpRes::status_code_map[506] = "Variant Also Negotiates";
	HttpRes::status_code_map[507] = "Insufficient Storage";
	HttpRes::status_code_map[508] = "Loop Detected";
	HttpRes::status_code_map[510] = "Not Extended";
	HttpRes::status_code_map[511] = "Network Authentication Required";
}

//------------------------------------------------------------------------------

std::ostream&	operator<<(std::ostream& os, Webserver& ref) {
	for (size_t idx=0;idx<ref.servers.size();idx++)
		os << ref.servers[idx] << std::endl;
	return (os);
}
