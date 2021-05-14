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
		string				_name;
		string				_root;
		set<string>			_error_page;
		set<string>			_index_page;
		set<string>			_method;
		size_t				_head_max;
		size_t				_body_max;
		bool				_autoindex;
		uint64_t			_timeout;

	private:
	public:
		ConfigServer();
		// ConfigServer(const ConfigServer& ref);
		ConfigServer(map<string, string>& config_server_map);
		// ConfigServer& operator=(const ConfigServer& ref);
		// ~ConfigServer();

		void				set_name(string& val);
		void				set_root(string& val);
		void				set_set(set<string>& page, string& val);
		void				set_index_page(string& val);
		void				set_error_page(string& val);
		void				set_method(string& val);
		void				set_head_max(string& val);
		void				set_body_max(string& val);
		void				set_autoindex(string& val);
		void				set_timeout(string& val);

		class ConfigElementInvalid: public std::exception
		{
			public: const char* what() const throw();	
		};
		class ConfigElementEmpty: public std::exception
		{
			public: const char* what() const throw();	
		};
};
