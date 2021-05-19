#include "../includes/Http.hpp"

/*##############################################################################
	
	Http

##############################################################################*/

/*==============================================================================
	Constructor & Destructor
==============================================================================*/
Http::Http(int fd)
: _file(0),
  _stream(0xffff, 0, 0),
  _content_length(0)
{
}
//------------------------------------------------------------------------------
Http::~Http()
{
}

/*==============================================================================
	Method
==============================================================================*/

bool	Http::get_next_line()
{
	return _stream.get_line(_line);
}


/*==============================================================================
	Getter
==============================================================================*/

map<string, string>&	Http::get_headers()
{
	return	_headers;
}
//------------------------------------------------------------------------------
Stream&					Http::get_stream()
{
	return	_stream;
}
//------------------------------------------------------------------------------
string&					Http::get_string()
{
	return	_line;
}
//------------------------------------------------------------------------------
int						Http::get_file_fd()
{
	return	_file->get_fd();
}
//------------------------------------------------------------------------------
const string&			Http::get_file_name()
{
	return	_file->get_name();
}
//------------------------------------------------------------------------------
size_t				Http::get_file_size()
{
	return	_file->size();
}
//------------------------------------------------------------------------------
const string&			Http::get_header(const string& key)
{
	static		string	empty_str;
	map<string, string>::iterator	it = _headers.find(key);
	if (it == _headers.end())
	{
		return empty_str;
	}
	else
	{
		return it->second;
	}
}

/*==============================================================================
	Setter
==============================================================================*/

void					Http::del_file()
{
	delete _file;
	_file = 0;
}
//------------------------------------------------------------------------------
void					Http::clear()
{
	_headers.clear();
	_protocol.clear();
	_line.clear();
	_stream.clear();
	del_file();
	_content_length = 0;
	_scheme.clear();
}

//------------------------------------------------------------------------------

void	Http::set_header(string& line)
{
	string::const_iterator	it_line = line.begin();
	string::const_iterator	it_end = line.end();
	string					key;
	string					val;

	ft::get_chr_token(line, it_line, key, ':');
	while (it_line != it_end && *it_line == ' ')
	{
		++it_line;
	}
	ft::str_meta_key(key);
	_headers[key] = string(it_line, it_end);
}