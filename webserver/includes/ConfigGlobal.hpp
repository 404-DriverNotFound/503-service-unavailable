#pragma once
#include "AConfig.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "Tokenizer.hpp"
#include "Utils.hpp"
#include "Stream.hpp"
#include "Time.hpp"
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
		typedef server_container::const_iterator	server_iterator;
		typedef port_container::const_iterator		port_iterator;

	/*==========================================================================
		Members
	==========================================================================*/
	private:
		size_t										_max_connection;
		Time										_timeout;
		Time										_select_timeout;
		string										_temp_dir;
		int											_worker;
		port_container								_ports;
		map<int, string>							_status_code;
		map<string, string>							_cgi_bin;

	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	public:
		ConfigGlobal();
		ConfigGlobal(int argc, char** argv, char** env);
		ConfigGlobal(const ConfigGlobal& ref);
		~ConfigGlobal();
		ConfigGlobal& operator=(const ConfigGlobal& ref);

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
		void				set_select_timeout(string& val);
		void				set_temp_dir(string& val);
		void				set_worker(string& val);
		void				set_status_code_map();
		void				set_path_cgi_bin(char** env);
	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		size_t					get_max_connection() const;
		const Time&				get_timeout() const;
		const Time&				get_select_timeout() const;
		const string&			get_temp_dir() const;
		int						get_worker() const;
		const port_container&	get_ports() const;
		const string&			get_status_code(int code) const;
		const server_container&	get_server(int port) const;
		const string&			get_cgi_bin(const string& extension) const;

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