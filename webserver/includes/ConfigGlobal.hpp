#pragma once
// #include "ClassModule.hpp"
#include "ConfigServer.hpp"
// #include "ConfigLocation.hpp"
#include "Stream.hpp"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <deque>

/*##############################################################################
Config Global
##############################################################################*/

using std::map;
using std::deque;
using std::string;

class ConfigGlobal
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	private:
		size_t						_max_connection;
		long						_timeout;
		string						_temp_file_dir;

	public:
		map<uint32_t, ConfigServer>	servers;
	
	/*--------------------------------------------------------------------------
	Constructor & Destructor
	--------------------------------------------------------------------------*/
	private:
		ConfigGlobal();
		ConfigGlobal(const ConfigGlobal& ref);
		ConfigGlobal& operator=(const ConfigGlobal& ref);
		
	public:
		ConfigGlobal(int argc, char** argv, char** env);
		~ConfigGlobal();

		int					open_file(int argc, char** argv, char** env);
		void				read_file(int fd, Stream& stream);
		deque<string>		get_lines(Stream& stream);
		void				parse(deque<string>& lines);
		void				set_global_config(map<string, string>& token_map);

		void				set_max_connection(string& val);
		void				set_timeout(string& val);
		void				set_temp_file_dir(string& val);


		map<string, string>	tokenizer_map(deque<string>::iterator& it, const string& stop);
		// ConfigLocation		set_location(map<string, string>, string);

	/*--------------------------------------------------------------------------
	Setter
	--------------------------------------------------------------------------*/
	public:
		// void				set_max_connection(map<string, string>, string& key);
		// void				set_timeout(map<string, string>, string& key);
		// void				set_temp_file_dir(map<string, string>, string& key);

	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	private:
		class ConfigFileOpenError: public std::exception
		{
			public: const char* what() const throw();	
		};
		class ConfigFileNoPort: public std::exception
		{
			public: const char* what() const throw();	
		};
};