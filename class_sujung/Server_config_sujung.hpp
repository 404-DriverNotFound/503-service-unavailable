#pragma once
#include "Location_sujung.hpp"

class Server_Config : public Location
{
	private:
		std::string					server_name;
		u_int16_t					port;
		std::vector<Location>		location;
	// method
	
	public:
		Server_Config();
};