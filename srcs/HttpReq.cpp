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
	if (!token.empty() && token.back() == '\r')
		token.pop_back();
	token.swap(protocol);
}

//------------------------------------------------------------------------------

void		HttpReq::set_headers(const std::string& line)
{
	std::string::const_iterator			it_line = line.begin();
	std::string							key;
	std::string							val;

	ft::get_set_token(line, it_line, key, ": ");
	while (ft::strchr(": ", *it_line))
		++it_line;
	headers[key].assign(it_line, line.end());
}

//------------------------------------------------------------------------------

void		HttpReq::set_method(const std::string& token)
{
	if (token == "CONNECT")
		method = CONNECT;
	else if (token == "DELETE")
		method = DELETE;
	else if (token == "GET")
		method = GET;
	else if (token == "HEAD")
		method = HEAD;
	else if (token == "OPTIONS")
		method = OPTIONS;
	else if (token == "PATCH")
		method = PATCH;
	else if (token == "POST")
		method = POST;
	else if (token == "PUT")
		method = PUT;
	else if (token == "TRACE")
		method = TRACE;
	else
		throw HttpFormException();
}
