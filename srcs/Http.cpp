#include "Http.hpp"

void		HttpReq::set_start_line(const std::string& line)
{
	std::string					token;
	std::string::const_iterator	it;

	get_chr_token(line, it, token, ' ');
	token.

}