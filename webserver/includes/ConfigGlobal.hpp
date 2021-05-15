#pragma once
#include "AConfig.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "Tokenizer.hpp"
#include "Utils.hpp"
#include "Stream.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>
#include <deque>

using std::cout;
using std::endl;
using std::map;
using std::deque;
using std::string;

/*##############################################################################
Config Global
##############################################################################*/

class ConfigGlobal
{
	/*==========================================================================
		Member Types
	==========================================================================*/
	public:
		typedef map<string, ConfigServer>			server_container;
		typedef map<uint32_t, server_container>		port_container;
		typedef server_container::iterator			server_iterator;
		typedef port_container::iterator			port_iterator;

	/*==========================================================================
		Members
	==========================================================================*/
	private:
		size_t										_max_connection;
		long										_timeout;
		string										_temp_dir;

	public:
		map<uint32_t, map<string, ConfigServer> >	ports;
	
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		ConfigGlobal();
		ConfigGlobal(const ConfigGlobal& ref);
		ConfigGlobal& operator=(const ConfigGlobal& ref);
		
	public:
		ConfigGlobal(int argc, char** argv, char** env);
		~ConfigGlobal();

	/*==========================================================================
		Method
	==========================================================================*/
	private:
		int					open_file(int argc, char** argv, char** env);
		void				read_file(int fd, Stream& stream);
		deque<string>		get_lines(Stream& stream);
		void				parse(deque<string>& lines);
		void				set_global_config(map<string, string>& token_map);
		map<string, string>	tokenizer_map(deque<string>::iterator& it, 
							deque<string>::iterator& end, const string& stop);

	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void				set_max_connection(string& val);
		void				set_timeout(string& val);
		void				set_temp_dir(string& val);
	
	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		size_t				get_max_connection();
		long				get_timeout();
		const string&		get_temp_dir();
	
	/*==========================================================================
		Exception
	==========================================================================*/
	private:
		class ConfigFileOpenError: public std::exception
		{	public: const char* what() const throw();	};

		class ConfigFileNoPort: public std::exception
		{	public: const char* what() const throw();	};
};

std::ostream&		operator<<(std::ostream& os, ConfigGlobal& conf);