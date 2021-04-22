#include "HttpReq.hpp"

/*##############################################################################
Http Request
##############################################################################*/

std::map<int, std::string>	status_code_map;

//------------------------------------------------------------------------------

void		HttpReq::set_start_line(const std::string& line)
{
	std::string::const_iterator			it_line = line.begin();
	std::string							token;

	// method
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw 400;
	set_method(token);
	
	//url
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw 400;
	std::string::const_iterator			it_token = token.begin();
	//	url
	ft::get_chr_token(token, it_token, url, '?');
	//	query
	query.assign(const_cast<char*>(it_token.base()), token.end().base());

	// scheme
	ft::get_chr_token(line, it_line, token, ' ');
	if (token.empty())
		throw 400;
	if (!token.empty() && *--token.end() == '\r')
		token.erase(--token.end());
	token.swap(protocol);
}

//------------------------------------------------------------------------------

void		HttpReq::set_headers(const std::string& line)
{
	std::string::const_iterator			it_line = line.begin();
	std::string							key;
	std::string							val;

	ft::get_set_token(line, it_line, key, ": ");
	ft::lowercase(key);
	while (ft::strchr(": ", *it_line))
		++it_line;
	headers[key].assign(it_line, line.end());
}

//------------------------------------------------------------------------------

void		HttpReq::get_location_name(std::string& location_name)
{
	std::string::const_iterator		it = url.begin();
	std::string						token;

	ft::get_chr_token(url, it, location_name, '/', 20);
}

//------------------------------------------------------------------------------

void		HttpReq::set_method(const std::string& token)
{
	std::map<std::string, uint16_t>::iterator it = Http::mapMethod.find(token);
	if (it == Http::mapMethod.end())
		throw 400;
	method = it->second;
}
