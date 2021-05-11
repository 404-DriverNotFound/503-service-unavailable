#pragma once
#include "ClassModule.hpp"
#include <map>

using std::map;

class Webserver
{
	private:
		const ConfigGlobal&		_config;

	private:
		Webserver();
		Webserver(const Webserver& ref);
		Webserver& operator=(const Webserver& ref);

	public:
		/*------------------------
		Constructor & Destructor
		------------------------*/
		Webserver(const ConfigGlobal& config);
		~Webserver();
		map<uint32_t, ConfigServer>				web_servers;
		/*------------------------
		getter
		------------------------*/

		/*------------------------
		setter
		------------------------*/

		// setter
		// method
		// exception
};