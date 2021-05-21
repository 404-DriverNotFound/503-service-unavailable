#include "../includes/ConfigServer.hpp"

/*------------------------------------------------------------------------------
Constructor & Destructor
------------------------------------------------------------------------------*/
ConfigServer::ConfigServer()
{}

//------------------------------------------------------------------------------
ConfigServer::ConfigServer(const ConfigServer& ref)
: AConfig(ref), _port(ref._port), _locations(ref._locations)
{}

ConfigServer&	ConfigServer::operator=(const ConfigServer& ref)
{
	if (&ref == this)
		return *this;
	AConfig::operator=(ref);
	_port = ref._port;
	// _locations = ref._locations;
	return *this;
}

//------------------------------------------------------------------------------
ConfigServer::ConfigServer(map<string, string>& config_server_map)
: AConfig(config_server_map)
{
	set_port		(	config_server_map["port"]		);
}
//------------------------------------------------------------------------------

ConfigServer::~ConfigServer()
{}

/*------------------------------------------------------------------------------
Setter
------------------------------------------------------------------------------*/
void			ConfigServer::set_port(string& val)
{
	if (val.empty())
	{
		// cout << "<port> ";
		throw ConfigElementEmpty();
	}
	_port = ft::atoi(val);
}

void			ConfigServer::set_location(string& name, ConfigLocation location)
{
	_locations[name] = location;
}

/*------------------------------------------------------------------------------
Getter
------------------------------------------------------------------------------*/

uint32_t			ConfigServer::get_port() const
{	return _port; }

const ConfigServer::location_container&	ConfigServer::get_locations() const
{	return _locations; }


std::ostream&		operator<<(std::ostream& os, const ConfigServer& conf)
{
	// cout << "Server Configurations:" << endl;

	// cout << static_cast<const AConfig&>(conf);
	// cout << std::setw(16) << "port: " << conf.get_port() << endl << endl;
	for (ConfigServer::location_const_iterator it = conf.get_locations().begin() ; it != conf.get_locations().end() ; ++it)
	{
		// cout << it->second << endl;
	}
	return os;
}