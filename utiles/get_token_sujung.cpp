#include <iostream>
#include <string>

bool	get_seq_token(std::string &origin, std::string::iterator &it, std::string &token, char *seq, size_t	reserve_size = 1000)
{
	std::string::iterator	end = origin.end();
	int						seq_len;

	seq_len = std::strlen(seq);
	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if (!strncmp(it.base(), seq, seq_len))
		{
			it += seq_len;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}

bool	get_set_token(std::string &origin, std::string::iterator &it, std::string &token, char *set, size_t	reserve_size = 1000)
{
	std::string::iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if (strchr(set, *it))
		{
			it += 1;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}

bool	get_chr_token(std::string &origin, std::string::iterator &it, std::string &token, char c, size_t reserve_size = 1000)
{
	std::string::iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while(it != end)
	{
		if (*it == c)
		{
			it += 1;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}
