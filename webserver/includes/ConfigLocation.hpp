#pragma once
#include "AConfig.hpp"
#include "Stream.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <set>
#include <map>

using std::string;
using std::set;
using std::map;
/*##############################################################################
Config Location
##############################################################################*/

class ConfigLocation: public AConfig
{
	/*==========================================================================
		Member Types
	==========================================================================*/	
	private:
		string				_auth_type;
		string				_auth;
		set<string>			_cgi_extensions;
	
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	public:
		ConfigLocation();
		ConfigLocation(const ConfigLocation& ref);
		ConfigLocation(map<string, string>& config_map, const string& root);
		~ConfigLocation();	
		ConfigLocation& operator=(const ConfigLocation& ref);

	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void				set_auth_type(string& val);
		void				set_auth(string& val);
		void				set_cgi_extensions(string& val);

	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		const string&		get_auth_type() const;
		const string&		get_auth() const;
		const set<string>&	get_cgi_extensions() const;

};

/*==============================================================================
	Non Member
==============================================================================*/
std::ostream&		operator<<(std::ostream& os, const ConfigLocation& conf);