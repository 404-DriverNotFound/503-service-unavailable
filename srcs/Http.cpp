#include "Http.hpp"

bool	get_chr_token(std::string &origin, std::string::iterator &it, std::string &token, char c, size_t reserve_size = 1000)
{
	std::string::iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while(it != end)
	{
		if (*it == c)
		{
			while (*it == c)
				it++;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}

			StartLine::StartLine()
{}

			StartLine::StartLine(e_message_type t, std::string& str)
: message_type(t)
{
	tokens.assign(4, std::string());
	std::string::iterator		it = str.begin();

	int i = 0;
	while (get_chr_token(str, it, tokens[i++], ' '))
		;

	for (std::string::iterator itk = tokens.begin() ; itk != tokens.end() ; ++itk)
		std::cout << *itk << std::endl;
}

int			main()
{
	std::string a("GET /html HTTP/1.1");
}