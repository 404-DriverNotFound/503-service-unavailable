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

	read_file(fd, stream);
	deque<string>	lines = get_lines(stream);
	
	parse(lines);
}

//------------------------------------------------------------------------------

ConfigGlobal::ConfigGlobal(const ConfigGlobal& ref)
: _max_connection(ref._max_connection),
_timeout(ref._timeout),
_temp_dir(ref._temp_dir),
_worker(ref._worker),
_ports(ref._ports)
{}

//------------------------------------------------------------------------------

ConfigGlobal::~ConfigGlobal()
{}

ConfigGlobal&	ConfigGlobal::operator=(const ConfigGlobal& ref)
{
	if (this == &ref)
		return *this;
	_max_connection = ref._max_connection;
	_timeout = ref._timeout;
	_temp_dir = ref._temp_dir;
	_worker = ref._worker;
	_ports = ref._ports;
	return *this;
}

//------------------------------------------------------------------------------

int				ConfigGlobal::open_file(int argc, char** argv, char** env)
{
	int		fd;

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

void			ConfigGlobal::read_file(int fd, Stream& stream)
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
			_ports[port][server_name].set_location(location_name, ConfigLocation(token_map));
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
	cout << "============================" << endl;
	cout << "Global Configurations:" << endl;
	cout << "----------------------------" << endl;
	cout << std::setw(16) << "max connection: "	<< conf.get_max_connection()	<< endl;
	cout << std::setw(16) << "temp dir: "		<< conf.get_temp_dir()			<< endl;
	cout << std::setw(16) << "timeout: "		<< conf.get_timeout()			<< endl;
	cout << std::setw(16) << "worker: "			<< conf.get_worker()			<< endl;

	ConfigGlobal::port_iterator	it_port = conf.get_ports().begin();
	while (it_port != conf.get_ports().end())
	{
		cout << "============================" << endl;
		cout << "port:" << it_port->first << endl;
		cout << "----------------------------" << endl;
		ConfigGlobal::server_iterator it_server = it_port->second.begin();
		while (it_server != it_port->second.end())
		{
			cout << it_server->second << endl;
			++it_server;
		}
		++it_port;
	}
	return os;
}