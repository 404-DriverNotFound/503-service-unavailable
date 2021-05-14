#include "../includes/ConfigGlobal.hpp"
#include "../includes/Tokenizer.hpp"
#include "../includes/Utils.hpp"

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

ConfigGlobal::~ConfigGlobal()
{}
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
//------------------------------------------------------------------------------

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
	// for (deque<string>::iterator it = lines.begin() ; it != lines.end() ; ++it)
	// 	cout << *it << endl;
	return lines;
}
//------------------------------------------------------------------------------

void			ConfigGlobal::parse(deque<string>& lines)
{
	for (deque<string>::iterator it = lines.begin() ; it != lines.end() ; ++it)
	{
		if (*it == "[global]")
		{
			++it;
			map<string, string>	token_map = tokenizer_map(it, "[server]");
			set_global_config(token_map);
		}
		if (*it == "[server]")
		{
			++it;
			map<string, string>	token_map = tokenizer_map(it, "[location]");
			string				str_port = "port";
			uint32_t			port= ft::atoi(ft::find_map<ConfigFileNoPort>(token_map, str_port));
			servers.insert(std::make_pair<uint32_t, ConfigServer>(port, ConfigServer(token_map)));
		}
	}
}

map<string, string>	ConfigGlobal::tokenizer_map(deque<string>::iterator& it, const string& stop)
{
	map<string, string>		token_map;
	while (*it != stop)
	{
		Tokenizer	tokenizer(*it);
		string	key = tokenizer.chr('\t');
		string	val = tokenizer.chr(0);
		cout << key << endl;
		cout << val << endl;
		token_map[key] = val;
		++it;
	}
	return token_map;
}

void				ConfigGlobal::set_global_config(map<string, string>& token_map)
{
	set_max_connection(token_map["max_connection"]);
	set_timeout(token_map["timeout"]);
	set_temp_file_dir(token_map["temp_file_dir"]);
}

void				ConfigGlobal::set_max_connection(string& val)
{
	_max_connection = ft::atoi(val);
}

void				ConfigGlobal::set_timeout(string& val)
{
	_timeout = ft::atoi(val);
}

void				ConfigGlobal::set_temp_file_dir(string& val)
{
	_temp_file_dir = val;
}





//------------------------------------------------------------------------------

const char*		ConfigGlobal::ConfigFileOpenError::what() const throw()
{
	return "config file open error";
}
//------------------------------------------------------------------------------

const char*		ConfigGlobal::ConfigFileNoPort::what() const throw()
{
	return "port";
}
//------------------------------------------------------------------------------