#include "Location_sujung.hpp"
#include "utils.hpp"

Location::Location(std::deque<std::string>& token)
{
	char	*base;

	base = token[0].begin().base();
	location += base + 10;
	token.pop_front();
	base = token[0].begin().base();
	while (!strncmp(base, "\t\t", 2))
	{
		base += 2;
		if (!strncmp(base, "root", 4))
		{
			root += (base + 5);
		}
		else if (!strncmp(base, "error_page", 10))
		{
			std::string::iterator	it;
			std::string				temp;

			it = token[0].begin() + 12;
			while (get_set_token(token[0], it, temp, " \t"))
			{
				error_page.push_back(temp);
			}
		}
		else if (!strncmp(base, "index", 5))
		{
			std::string::iterator	it;
			std::string				temp;

			it = token[0].begin() + 7;
			while (get_set_token(token[0], it, temp, " \t"))
			{
				index.push_back(temp);
			}
		}
		else if (!strncmp(base, "method", 6))
		{
			std::string::iterator	it;
			std::string				temp;

			it = token[0].begin() + 8;
			while (get_set_token(token[0], it, temp, " \t"))
			{
				method.push_back(temp);
			}
		}
		else if (!strncmp(base, "body_length", 11))
		{
			body_length = static_cast<u_int16_t>(atoi(base + 12));
		}
		else if (!strncmp(base, "autoindex", 8))
		{
			if (!strncmp(base + 9, " on", 0))
				autoindex = true;
			else
				autoindex = false;
		}
		token.pop_front();
		base = token[0].begin().base();
	}
}

Location::~Location()
{
}