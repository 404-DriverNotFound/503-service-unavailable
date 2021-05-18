#include "../includes/ConfigLocation.hpp"


//------------------------------------------------------------------------------
ConfigLocation::ConfigLocation()
{}

//------------------------------------------------------------------------------

ConfigLocation::ConfigLocation(const ConfigLocation& ref)
: AConfig(ref), 
_auth_type(ref._auth_type),
_auth(ref._auth),
_cgi_extensions(ref._cgi_extensions)
{}

//------------------------------------------------------------------------------

ConfigLocation::ConfigLocation(map<string, string>& config_map, const string& root)
: AConfig(config_map)
{
	set_auth_type(config_map["auth_type"]);
	set_auth(config_map["auth"]);
	set_cgi_extensions(config_map["cgi_extension"]);
	_root += root;
}

//------------------------------------------------------------------------------

ConfigLocation::~ConfigLocation()
{}

//------------------------------------------------------------------------------

ConfigLocation& ConfigLocation::operator=(const ConfigLocation& ref)
{
	if (&ref == this)
		return *this;
	AConfig::operator=(ref);
	_auth_type = ref._auth_type;
	_auth = ref._auth;
	_cgi_extensions = ref._cgi_extensions;
	return *this;
}


void				ConfigLocation::set_auth_type(string& val)
{
	_auth_type = val;
}

void				ConfigLocation::set_auth(string& val)
{
	if (val.empty())
		return;
	int		fd = open(val.c_str(), O_RDONLY);
	if (fd < 0)
	{
		cout << "<auth file> ";
		throw ConfigElementInvalid();
	}
	Stream		stream(ft::file_size(val.c_str()), fd);
	stream.fill(ft::file_size(val.c_str()));
	stream.get_chr_token(_auth, 0);
	close(fd);
	if (_auth_type.empty() && !_auth.empty())
		_auth_type = "basic";
}

void				ConfigLocation::set_cgi_extensions(string& val)
{
	set_set(_cgi_extensions, val);
}

const string&		ConfigLocation::get_auth_type() const
{
	return _auth_type;
}

const string&		ConfigLocation::get_auth() const
{
	return _auth;
}

const set<string>&	ConfigLocation::get_cgi_extensions() const
{
	return _cgi_extensions;
}



//------------------------------------------------------------------------------

std::ostream&		operator<<(std::ostream& os, const ConfigLocation& conf)
{
	cout << "Location Configurations:" << endl;
	
	cout << static_cast<const AConfig&>(conf);
	cout << std::setw(16) << "auth_type: "			<< conf.get_auth_type() << endl;	
	cout << std::setw(16) << "auth: "				<< conf.get_auth() << endl;
	cout << std::setw(16) << "cgi_extensions: "		<< conf.get_cgi_extensions() << endl;

	return os;
}