#include "utils_minckim.hpp"

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

//------------------------------------------------------------------------------
					Buffer::Buffer(int fd, size_t buffer_size)
: fd(fd), buffer_size(buffer_size)
{
	buffer = new char[buffer_size + 1];
	cursor = buffer;
	end = buffer;
}

//------------------------------------------------------------------------------
					Buffer::~Buffer()
{
	delete[] buffer;
}

//------------------------------------------------------------------------------
int					Buffer::read_buffer()
{
	ssize_t		len = -1;

	len = read(fd, buffer, buffer_size);
	end = buffer + len;
	cursor = buffer;
	// write(1, buffer, len);
	return len;
}

//------------------------------------------------------------------------------
int					Buffer::get_token(std::string& token, int seq)
{
	if (flag_token == 1)
		token.clear();
	while (cursor != end)
	{
		if (seq == *cursor)
		{
			flag_token = 1;
			cursor++;
			return 1;
		}
		token.push_back(*cursor++);
	}
	flag_token = 0;
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
