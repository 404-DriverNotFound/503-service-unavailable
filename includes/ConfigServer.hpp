#pragma once
#include <string>
#include <map>
#include <set>

using std::string;
using std::map;
using std::set;

class ConfigServer
{
	private:
		uint32_t			_port;
		string				_temp_file_dir;
		string				_name;
		string				_root;
		set<string>			_error_page;
		set<string>			_index;
		uint32_t			_method;
		uint32_t			_head_length;
		uint64_t			_body_length;
		bool				_autoindex;
		uint64_t			_timeout;

	private:
		ConfigServer();
		ConfigServer(const ConfigServer& ref);
		ConfigServer& operator=(const ConfigServer& ref);

	public:
		~ConfigServer();
};
