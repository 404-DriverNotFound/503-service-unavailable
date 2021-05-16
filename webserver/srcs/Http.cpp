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
