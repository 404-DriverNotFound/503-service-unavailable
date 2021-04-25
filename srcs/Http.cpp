#include "../includes/Http.hpp"

/*##############################################################################
Http
##############################################################################*/

std::map<std::string, u_int16_t>	Http::map_method;
std::map<std::string, u_int16_t>	Http::map_header;

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
	Http::map_header["ACCEPT_CHARSET"]		 = ACCEPT_CHARSET;
	Http::map_header["ACCEPT_LANGUAGE"]		 = ACCEPT_LANGUAGE;
	Http::map_header["AUTHORIZATION"]		 = AUTHORIZATION;
	Http::map_header["HOST"]				 = HOST;
	Http::map_header["REFERER"]				 = REFERER;
	Http::map_header["USER_AGENT"]			 = USER_AGENT;
	Http::map_header["LAST_MODIFIED"]		 = LAST_MODIFIED;
	Http::map_header["LOCATION"]			 = LOCATION;
	Http::map_header["RETRY_AFTER"]			 = RETRY_AFTER;
	Http::map_header["SERVER"]				 = SERVER;
	Http::map_header["TRANSFER_ENCODING"]	 = TRANSFER_ENCODING;
	Http::map_header["WWW_AUTHENTICATE"]	 = WWW_AUTHENTICATE;
	Http::map_header["ALLOW"]				 = ALLOW;
	Http::map_header["CONTNET_LENGTH"]		 = CONTNET_LENGTH;
	Http::map_header["CONTENT_LOCATION"]	 = CONTENT_LOCATION;
	Http::map_header["CONTENT_TYPE"]		 = CONTENT_TYPE;
	Http::map_header["DATE"]				 = DATE;
	Http::map_header["NUM_HEADERS"]			 = NUM_HEADERS;
}

//------------------------------------------------------------------------------
