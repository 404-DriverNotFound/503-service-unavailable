#pragma once
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>

/*##############################################################################
Config Global
##############################################################################*/

using std::map;
using std::string;

class ConfigGlobal
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	private:
		size_t				_max_connection;
		long				_timeout;
		string				_temp_file_dir;
	
	/*--------------------------------------------------------------------------
	Constructor & Destructor
	--------------------------------------------------------------------------*/
	private:
		ConfigGlobal();
		ConfigGlobal(const ConfigGlobal& ref);
		ConfigGlobal&		operator=(const ConfigGlobal& ref);
		
	public:
		ConfigGlobal(int argc, char** argv, char** env);
		~ConfigGlobal();

	/*--------------------------------------------------------------------------
	Setter
	--------------------------------------------------------------------------*/
	public:
		void			set_max_connection(map<string, string>, string& key);
		void			set_timeout(map<string, string>, string& key);
		void			set_temp_file_dir(map<string, string>, string& key);



};