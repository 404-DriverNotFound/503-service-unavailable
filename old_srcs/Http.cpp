#include "../includes/Http.hpp"

/*##############################################################################
Http
##############################################################################*/

std::map<std::string, u_int32_t>	Http::map_method;

Http::Http(int sock_fd)
: stream(0xffffff, sock_fd, sock_fd), content_length(0)
{}

//------------------------------------------------------------------------------

void		Http::set_header(const string& line)
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
	headers[key] = string(it_line, it_end);
}

//------------------------------------------------------------------------------

const char*		Http::HttpFormException::what() const throw()
{	return "HttpFormException";	}

void	Http::init_map_methods()
{
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("CONNECT", CONNECT));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("DELETE", DELETE));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("GET", GET));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("HEAD", HEAD));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("OPTIONS", OPTIONS));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("PATCH", PATCH));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("POST", POST));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("PUT", PUT));
	Http::map_method.insert(std::make_pair<std::string, uint32_t>("TRACE", TRACE));
}

//------------------------------------------------------------------------------
