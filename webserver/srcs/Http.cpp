#include "../includes/Http.hpp"

Http::Http()
{
}

Http::~Http()
{
}

map<string, string>&	Http::get_headers()
{
	return	_headers;
}

Stream&					Http::get_stream()
{
	return	_stream;
}

string&					Http::get_string()
{
	return	_line;
}

void					Http::set_file(File::flag f = File::flag::o_create)
{
	_file = new File(f);
}

void					Http::set_file(const string& path, File::flag f = File::flag::o_read)
{
	_file = new File(path, f);
}
