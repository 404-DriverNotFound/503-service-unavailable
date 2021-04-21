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

	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw HttpFormException();
	set_method(token);
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw HttpFormException();
	token.swap(path);
	ft::get_chr_token(line, it_line, token, ' ');
	if (token.empty())
		throw HttpFormException();
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
	std::string::const_iterator		it;
	ft::get_chr_token(path, it, location_name, '/', 20);
}

//------------------------------------------------------------------------------

void		HttpReq::set_method(const std::string& token)
{
	std::map<std::string, uint16_t>::iterator it = Http::mapMethod.find(token);
	if (it == Http::mapMethod.end())
		throw 400;
	method = it->second;
}
