#pragma once
#include <string>

class ConfigGlobal
{
	private:
		size_t				_max_connection;
		long				_timeout;
		
	private:
		ConfigGlobal();
		ConfigGlobal(const ConfigGlobal& ref);
		ConfigGlobal&		operator=(const ConfigGlobal& ref);
		
	public:
		ConfigGlobal(int argc, char** argv, char** env);
		~ConfigGlobal();
};