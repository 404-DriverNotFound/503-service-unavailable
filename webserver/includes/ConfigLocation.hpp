#pragma once
#include "ClassModule.hpp"
#include <string>
#include <set>

using std::string;
using std::set;

class ConfigLocation
{
	private:
		string			_auth_type;
		string			_auth;
		set<string>		_cgi_extensions;
		string			_name;
		Path			_path;
		set<string>		_index;
		size_t			_max_head;
		size_t			_max_body;
		bool			_autoindex;
		long			_timeout;
	
	private:
		ConfigLocation();
		ConfigLocation(const ConfigLocation& ref);
		ConfigLocation& operator=(const ConfigLocation& ref);

	public:
		~ConfigLocation();	
};