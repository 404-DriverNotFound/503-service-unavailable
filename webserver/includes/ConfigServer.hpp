#pragma once
#include "AConfig.hpp"
#include "ConfigLocation.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <set>

using std::string;
using std::map;
using std::set;

/*##############################################################################
Config Server
##############################################################################*/

class ConfigServer: public AConfig
{
	/*==========================================================================
	Member Types
	==========================================================================*/
	public:
		typedef map<string, ConfigLocation>			location_container;
		typedef location_container::iterator		location_iterator;
		typedef location_container::const_iterator	location_const_iterator;

	/*==========================================================================
	Member
	==========================================================================*/
	private:
		uint32_t				_port;
		location_container		_locations;

	/*==========================================================================
	Constructor & Destructor
	==========================================================================*/
	public:
		ConfigServer();
		ConfigServer(const ConfigServer& ref);
		ConfigServer(map<string, string>& config_server_map);
		ConfigServer& operator=(const ConfigServer& ref);
		~ConfigServer();

	/*==========================================================================
	Method
	==========================================================================*/
	private:
		void				set_set(set<string>& page, string& val);

	/*==========================================================================
	Setter
	==========================================================================*/
	public:
		void				set_port(string& val);
		void				set_location(string& name, ConfigLocation location);

	/*==========================================================================
	Getter
	==========================================================================*/
	public:
		uint32_t					get_port() const;
		const location_container&	get_locations() const;
};

/*==============================================================================
	Non Member
==============================================================================*/
std::ostream&		operator<<(std::ostream& os, const ConfigServer& conf);