#pragma once
#include <vector>
#include "Config.hpp"
#include "Path.hpp"
#include "Cgi.hpp"
#include "Http.hpp"

/*
*Webserver
*>> Server
*>>>> Location
*>>>> Location
*>>>> Location
*>> Server
*>>>> Location
*>>>> Location
*>> Server
*>>>> Location
*>>>> Location
*>>>> Location
*	.
*	.
*	.
*/
struct ConfigWebserver
{
	public:
	uint32_t							max_connection;
	timeval								select_timeout;
	std::vector<std::vector<Config> >	config_servers;		

				ConfigWebserver(const Path&);
	void		set_path_cgi_bin(char** env);
	void		set_status_code();
};