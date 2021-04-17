#pragma once
#include <vector>
#include "Config.hpp"
#include "Path.hpp"

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
class ConfigWebserver
{
	public:
	uint32_t							max_connection;
	timeval								select_timeout;
	std::vector<std::vector<Config> >	config_servers;		

				ConfigWebserver(const Path&);
	void		set_path_cig_bin();
};