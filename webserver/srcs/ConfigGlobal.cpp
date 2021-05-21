#include "../includes/ConfigGlobal.hpp"

/*==============================================================================
	Constructor & Destructor
==============================================================================*/
ConfigGlobal::ConfigGlobal()
{}

//------------------------------------------------------------------------------

ConfigGlobal::ConfigGlobal(int argc, char** argv, char** env)
{
	int			fd = open_file(argc, argv, env);
	Stream		stream(10000, fd);

	read_file(stream);
	deque<string>	lines = get_lines(stream);
	
	parse(lines);
	set_status_code_map();
	set_path_cgi_bin(env);
}

//------------------------------------------------------------------------------

ConfigGlobal::ConfigGlobal(const ConfigGlobal& ref)
: _max_connection(ref._max_connection),
_timeout(ref._timeout),
_select_timeout(ref._select_timeout),
_temp_dir(ref._temp_dir),
_worker(ref._worker),
_ports(ref._ports),
_status_code(ref._status_code),
_cgi_bin(ref._cgi_bin)
{
}

//------------------------------------------------------------------------------

ConfigGlobal::~ConfigGlobal()
{}

ConfigGlobal&	ConfigGlobal::operator=(const ConfigGlobal& ref)
{
	if (this == &ref)
		return *this;
	_max_connection = ref._max_connection;
	_timeout = ref._timeout;
	_select_timeout = ref._select_timeout;
	_temp_dir = ref._temp_dir;
	_worker = ref._worker;
	_ports = ref._ports;
	_status_code = ref._status_code;
	_cgi_bin = ref._cgi_bin;
	return *this;
}

//------------------------------------------------------------------------------

int				ConfigGlobal::open_file(int argc, char** argv, char** env)
{
	int		fd;
	(void)env;

	if (argc == 1)
	{
		fd = open("server_config", O_RDONLY);
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
	}
	if (fd < 0)
		throw ConfigFileOpenError();
	return fd;
}

/*==============================================================================
	Method
==============================================================================*/

void			ConfigGlobal::read_file(Stream& stream)
{
	while (stream.fill(10000));
}

//------------------------------------------------------------------------------

deque<string>	ConfigGlobal::get_lines(Stream& stream)
{
	deque<string>	lines;
	string			line;

	while (stream.get_line(line))
	{
		lines.push_back(string());
		lines.back().swap(line);
	}
	return lines;
}

//------------------------------------------------------------------------------

void			ConfigGlobal::parse(deque<string>& lines)
{
	string						str_name = "name";
	uint32_t					port;
	string						server_name;
	deque<string>::iterator		end = lines.end();

	for (deque<string>::iterator it = lines.begin() ; it != end ; ++it)
	{
		if (it->empty())
			continue;
		else if (*it == "[global]")
		{
			++it;
			if (it == end)
				break;
			map<string, string>	token_map = tokenizer_map(it, end, "");
			set_global_config(token_map);
			if (it == end)
				break;
		}
		else if (*it == "[server]")
		{
			++it;
			if (it == end)
				break;
			map<string, string>	token_map = tokenizer_map(it, end, "");
			map<string, string>::iterator	it_token_map = token_map.find("port");
			if (it_token_map == token_map.end())
				token_map["port"] = "80";
			port = ft::atoi(token_map["port"]);
			server_name = ft::find_map<ConfigServer::ConfigElementEmpty>(token_map, str_name);
			_ports[port][server_name] = ConfigServer(token_map);
			if (it == end)
				break;
		}
		else if (*it == "[location]")
		{
			++it;
			if (it == end)
				break;
			map<string, string>	token_map = tokenizer_map(it, end, "");
			string	location_name
			 = ft::find_map<ConfigServer::ConfigElementEmpty>(token_map, str_name);
			ConfigServer&	server = _ports[port][server_name];
			server.set_location(location_name, ConfigLocation(token_map, server.get_root()));
			if (it == end)
				break;
		}
	}
}

//------------------------------------------------------------------------------

map<string, string>	ConfigGlobal::tokenizer_map(deque<string>::iterator& it, 
deque<string>::iterator& end, const string& stop)
{
	map<string, string>		token_map;
	while (it != end && *it != stop)
	{
		Tokenizer	tokenizer(*it);
		string	key = tokenizer.chr('\t');
		string	val = tokenizer.rest();
		token_map[key] = val;
		++it;
	}
	return token_map;
}

/*==============================================================================
	Setter
==============================================================================*/

void				ConfigGlobal::set_global_config(map<string, string>& token_map)
{
	set_max_connection(token_map["max_connection"]);
	set_timeout(token_map["timeout"]);
	set_select_timeout(token_map["select_timeout"]);
	set_temp_dir(token_map["temp_dir"]);
	set_worker(token_map["worker"]);
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_max_connection(string& val)
{
	_max_connection = ft::atoi(val);
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_timeout(string& val)
{
	_timeout = Time(ft::atoi(val) * 1000000);
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_select_timeout(string& val)
{
	_select_timeout = Time(ft::atoi(val) * 1000000);
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_temp_dir(string& val)
{
	_temp_dir = val;
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_worker(string& val)
{
	_worker = ft::atoi(val);
}

//------------------------------------------------------------------------------

void				ConfigGlobal::set_status_code_map()
{
	_status_code[100] = "Continue";
	_status_code[101] = "Switching Protocols";
	_status_code[103] = "Early Hints";
	_status_code[200] = "OK";
	_status_code[201] = "Created";
	_status_code[202] = "Accepted";
	_status_code[203] = "Non-Authoritative Information";
	_status_code[204] = "No Content";
	_status_code[205] = "Reset Content";
	_status_code[206] = "Partial Content";
	_status_code[300] = "Multiple Choices";
	_status_code[301] = "Moved Permanently";
	_status_code[302] = "Found";
	_status_code[303] = "See Other";
	_status_code[304] = "Not Modified";
	_status_code[307] = "Temporary Redirect";
	_status_code[308] = "Permanent Redirect";
	_status_code[400] = "Bad Request";
	_status_code[401] = "Unauthorized";
	_status_code[402] = "Payment Required";
	_status_code[403] = "Forbidden";
	_status_code[404] = "Not Found";
	_status_code[405] = "Method Not Allowed";
	_status_code[406] = "Not Acceptable";
	_status_code[407] = "Proxy Authentication Required";
	_status_code[408] = "Request Timeout";
	_status_code[409] = "Conflict";
	_status_code[410] = "Gone";
	_status_code[411] = "Length Required";
	_status_code[412] = "Precondition Failed";
	_status_code[413] = "Payload Too Large";
	_status_code[414] = "URI Too Long";
	_status_code[415] = "Unsupported Media Type";
	_status_code[416] = "Range Not Satisfiable";
	_status_code[417] = "Expectation Failed";
	_status_code[418] = "I'm a teapot";
	_status_code[422] = "Unprocessable Entity";
	_status_code[425] = "Too Early";
	_status_code[426] = "Upgrade Required";
	_status_code[428] = "Precondition Required";
	_status_code[429] = "Too Many Requests";
	_status_code[431] = "Request Header Fields Too Large";
	_status_code[451] = "Unavailable For Legal Reasons";
	_status_code[500] = "Internal Server Error";
	_status_code[501] = "Not Implemented";
	_status_code[502] = "Bad Gateway";
	_status_code[503] = "Service Unavailable";
	_status_code[504] = "Gateway Timeout";
	_status_code[505] = "HTTP Version Not Supported";
	_status_code[506] = "Variant Also Negotiates";
	_status_code[507] = "Insufficient Storage";
	_status_code[508] = "Loop Detected";
	_status_code[510] = "Not Extended";
	_status_code[511] = "Network Authentication Required";
}
//------------------------------------------------------------------------------
void			ConfigGlobal::set_path_cgi_bin(char** env)
{
	_cgi_bin[".php"] = ft::which("php", env);
	_cgi_bin[".py"] = ft::which("python3", env);
	#ifdef __APPLE__
	_cgi_bin[".bla"] = "./test/cgi_tester";
	#else
	_cgi_bin[".bla"] = "./test/ubuntu_cgi_tester";
	#endif
}

/*==============================================================================
	Getter
==============================================================================*/

size_t				ConfigGlobal::get_max_connection() const
{
	return _max_connection;
}

//------------------------------------------------------------------------------

const Time&			ConfigGlobal::get_timeout() const
{
	return _timeout;
}

//------------------------------------------------------------------------------

const Time&			ConfigGlobal::get_select_timeout() const
{
	return _select_timeout;
}

//------------------------------------------------------------------------------

const string&		ConfigGlobal::get_temp_dir() const
{
	return _temp_dir;
}

//------------------------------------------------------------------------------

int					ConfigGlobal::get_worker() const
{
	return _worker;
}

//------------------------------------------------------------------------------

const ConfigGlobal::port_container&	ConfigGlobal::get_ports() const
{
	return _ports;
}

//------------------------------------------------------------------------------

const ConfigGlobal::server_container&	ConfigGlobal::get_server(int port) const
{
	return _ports.find(port)->second;
}

//------------------------------------------------------------------------------

const string&		ConfigGlobal::get_status_code(int code) const
{
	return _status_code.find(code)->second;
}

//------------------------------------------------------------------------------

const string&		ConfigGlobal::get_cgi_bin(const string& extension) const
{
	static string	tmp;
	map<string, string>::const_iterator it = _cgi_bin.find(extension);
	if (it != _cgi_bin.end())
		return it->second;
	return tmp;
}

/*==============================================================================
Exceptions
==============================================================================*/

const char*		ConfigGlobal::ConfigFileOpenError::what() const throw()
{
	return "config file open error";
}

//------------------------------------------------------------------------------

const char*		ConfigGlobal::ConfigFileNoPort::what() const throw()
{
	return "port";
}

/*==============================================================================
Nonmember
==============================================================================*/

std::ostream&		operator<<(std::ostream& os, ConfigGlobal& conf)
{
	// cout << "============================" << endl;
	// cout << "Global Configurations:" << endl;
	// cout << "----------------------------" << endl;
	// cout << std::setw(16) << "max connection: "	<< conf.get_max_connection()	<< endl;
	// cout << std::setw(16) << "temp dir: "		<< conf.get_temp_dir()			<< endl;
	// cout << std::setw(16) << "timeout: "		<< conf.get_timeout()			<< endl;
	// cout << std::setw(16) << "worker: "			<< conf.get_worker()			<< endl;
	// cout << std::setw(16) << "sel timeout: "	<< conf.get_select_timeout()	<< endl;

	ConfigGlobal::port_iterator	it_port = conf.get_ports().begin();
	while (it_port != conf.get_ports().end())
	{
		// cout << "============================" << endl;
		// cout << "port:" << it_port->first << endl;
		// cout << "----------------------------" << endl;
		ConfigGlobal::server_iterator it_server = it_port->second.begin();
		while (it_server != it_port->second.end())
		{
			// cout << it_server->second << endl;
			++it_server;
		}
		++it_port;
	}
	return os;
}
