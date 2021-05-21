#include "../includes/AConfig.hpp"

/*------------------------------------------------------------------------------
Constructor & Destructor
------------------------------------------------------------------------------*/
AConfig::AConfig()
{}

AConfig::AConfig(const AConfig& ref)
: _name(ref._name),
_root(ref._root),
_index_page(ref._index_page),
_error_page(ref._error_page),
_method(ref._method),
_head_max(ref._head_max),
_body_max(ref._body_max),
_autoindex(ref._autoindex)
{}

AConfig& AConfig::operator=(const AConfig& ref)
{
	if (&ref == this)
		return *this;
	_name = ref._name;
	_root = ref._root;
	_index_page = ref._index_page;
	_error_page = ref._error_page;
	_method = ref._method;
	_head_max = ref._head_max;
	_body_max = ref._body_max;
	_autoindex = ref._autoindex;
	return *this;
}

AConfig::~AConfig()
{}

//------------------------------------------------------------------------------

AConfig::AConfig(map<string, string>& config_server_map)
{
	set_name		(	config_server_map["name"]		);
	set_root		(	config_server_map["root"]		);
	set_index_page	(	config_server_map["index_page"]	);
	set_error_page	(	config_server_map["error_page"]	);
	set_method		(	config_server_map["method"]		);
	set_head_max	(	config_server_map["head_max"]	);
	set_body_max	(	config_server_map["body_max"]	);
	set_autoindex	(	config_server_map["autoindex"]	);
}

/*------------------------------------------------------------------------------
Method
------------------------------------------------------------------------------*/

void			AConfig::set_set(set<string>& container, string& val)
{

	Tokenizer	tokenizer(val);
	while (42)
	{
		string	token = tokenizer.chr(' ');
		if (token.empty())
			break;
		container.insert(token);
	}
}

/*------------------------------------------------------------------------------
Setter
------------------------------------------------------------------------------*/
void			AConfig::set_name(string& val)
{
	if (val.empty())
	{
		cout << _name;
		cout << "<name> ";
		throw ConfigElementEmpty();
	}
	_name.swap(val);
}
//------------------------------------------------------------------------------

void			AConfig::set_root(string& val)
{
	if (val.empty())
	{
		cout << _name;
		cout << ": <root> ";
		throw ConfigElementEmpty();
	}
	_root.swap(val);
}
//------------------------------------------------------------------------------

void			AConfig::set_index_page(string& val)
{
	set_set(_index_page, val);
}
//------------------------------------------------------------------------------

void			AConfig::set_error_page(string& val)
{
	set_set(_error_page, val);
}
//------------------------------------------------------------------------------

void			AConfig::set_method(string& val)
{
	set_set(_method, val);
}
//------------------------------------------------------------------------------

void			AConfig::set_head_max(string& val)
{
	if (val.empty())
	{
		_head_max = 0x2000;
		return;
	}
	_head_max = ft::atoi(val);
}
//------------------------------------------------------------------------------

void			AConfig::set_body_max(string& val)
{
	if (val.empty())
	{
		_head_max = 0x20000;
		return;
	}
	_body_max = ft::atoi(val);
}
//------------------------------------------------------------------------------

void			AConfig::set_autoindex(string& val)
{
	if (val.empty())
	{
		_autoindex = false;
		return;
	}
	if (val == "on")
		_autoindex = true;
	else if (val == "off")
		_autoindex = false;
	else
		throw ConfigElementInvalid();
	
}

/*------------------------------------------------------------------------------
Getter
------------------------------------------------------------------------------*/

const string&		AConfig::get_name() const
{	return _name; }

const string&		AConfig::get_root() const
{	return _root; }

const set<string>&	AConfig::get_index_page() const
{	return _index_page; }

const set<string>&	AConfig::get_error_page() const
{	return _error_page; }

const set<string>&	AConfig::get_method() const
{	return _method; }

size_t				AConfig::get_head_max() const
{	return _head_max; }

size_t				AConfig::get_body_max() const
{	return _body_max; }

bool				AConfig::get_autoindex() const
{	return _autoindex; }




/*------------------------------------------------------------------------------
Exception
------------------------------------------------------------------------------*/

const char*		AConfig::ConfigElementEmpty::what() const throw()
{
	return "Element not found.";
}

const char*		AConfig::ConfigElementInvalid::what() const throw()
{
	return "Invalid elemnet.";
}

std::ostream&		operator<<(std::ostream& os, const AConfig& conf)
{
	cout << std::setw(16) << "name: " << conf.get_name() << endl;
	cout << std::setw(16) << "root: " << conf.get_root() << endl;
	cout << std::setw(16) << "index_page: " << conf.get_index_page() << endl;
	cout << std::setw(16) << "error_page: " << conf.get_error_page() << endl;
	cout << std::setw(16) << "method: " << conf.get_method() << endl;
	cout << std::setw(16) << "head_max: " << conf.get_head_max() << endl;
	cout << std::setw(16) << "body_max: " << conf.get_body_max() << endl;
	cout << std::setw(16) << "autoindex: " << conf.get_autoindex() << endl;
	return os;
}