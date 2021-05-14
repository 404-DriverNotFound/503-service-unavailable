#pragma once
#include "ClassModule.hpp"
#include <map>

using std::map;
using std::string;

class Webserver
{
	public:
		static FdSet			o_set;
		static FdSet			r_set;
		static FdSet			w_set;
		static FdSet			e_set;

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
		map<uint32_t, map<string, ConfigServer> >	web_servers;
		/*------------------------
		getter
		------------------------*/

		/*------------------------
		setter
		------------------------*/


		/*------------------------
		Method
		------------------------*/
		void		start_server();
		
};