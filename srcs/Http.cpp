#include "../includes/Http.hpp"

/*##############################################################################
Http
##############################################################################*/

std::map<std::string, u_int16_t>	Http::map_method;
std::map<std::string, u_int16_t>	Http::map_header;

Http::Http(int sock_fd)
: stream(0x10000, sock_fd, sock_fd)
{}

//------------------------------------------------------------------------------

void		Http::set_header(const string& line)
{
	string::const_iterator	it_line = line.begin();
	string					key;
	string					val;

	ft::get_set_token(line, it_line, key, ": ");
	ft::uppercase(key);
	while (ft::strchr(": ", *it_line))
		++it_line;
	map<string, uint16_t>::iterator	it_header = map_header.find(key);
	if (it_header == map_header.end())
		return;
		// throw 400;
	headers[it_header->second] = it_line.base();
	if (key == "CONTENT-LENGTH")
		content_length = ft::atoi_hex(val);
}

//------------------------------------------------------------------------------

const char*		Http::HttpFormException::what() const throw()
{	return "HttpFormException";	}

void	Http::init_map_methods()
{
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("CONNECT", CONNECT));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("DELETE", DELETE));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("GET", GET));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("HEAD", HEAD));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("OPTIONS", OPTIONS));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("PATCH", PATCH));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("POST", POST));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("PUT", PUT));
	Http::map_method.insert(std::make_pair<std::string, uint16_t>("TRACE", TRACE));
}

//------------------------------------------------------------------------------

void		Http::init_map_headers()
{
	Http::map_header["ACCEPT-CHARSET"]		 = ACCEPT_CHARSET;
	Http::map_header["ACCEPT-LANGUAGE"]		 = ACCEPT_LANGUAGE;
	Http::map_header["AUTHORIZATION"]		 = AUTHORIZATION;
	Http::map_header["HOST"]				 = HOST;
	Http::map_header["REFERER"]				 = REFERER;
	Http::map_header["USER-AGENT"]			 = USER_AGENT;
	Http::map_header["LAST-MODIFIED"]		 = LAST_MODIFIED;
	Http::map_header["LOCATION"]			 = LOCATION;
	Http::map_header["RETRY-AFTER"]			 = RETRY_AFTER;
	Http::map_header["SERVER"]				 = SERVER;
	Http::map_header["TRANSFER-ENCODING"]	 = TRANSFER_ENCODING;
	Http::map_header["WWW-AUTHENTICATE"]	 = WWW_AUTHENTICATE;
	Http::map_header["ALLOW"]				 = ALLOW;
	Http::map_header["CONTNET-LENGTH"]		 = CONTNET_LENGTH;
	Http::map_header["CONTENT-LOCATION"]	 = CONTENT_LOCATION;
	Http::map_header["CONTENT-TYPE"]		 = CONTENT_TYPE;
	Http::map_header["DATE"]				 = DATE;
	Http::map_header["STATUS"]			 	 = STATUS;
	Http::map_header["NUM-HEADERS"]			 = NUM_HEADERS;
}

//------------------------------------------------------------------------------