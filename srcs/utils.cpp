#include "utils.hpp"

//------------------------------------------------------------------------------
size_t				strlen(const char* str)
{
	const char*		head = str;

	while (*str++)
	{}
	return str - head;
}

//------------------------------------------------------------------------------
int					strncmp(char* a, char* b, size_t len)
{
	while (len-- && *a && *b)
	{
		if (a != b)
			return a - b;
		a++;
		b++;
	}
	return 0;
}

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

int	get_set_token(std::string &origin, std::string::iterator &it, std::string &token, char *set, size_t	reserve_size = 1000)
{
	char	*p;
	std::string::iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if ((p = strchr(set, *it)))
		{
			it += 1;
			return *p;
		}
		token.push_back(*it);
		++it;
	}
	return 0;
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
			while (it != end && *it == c)
				++it;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}
