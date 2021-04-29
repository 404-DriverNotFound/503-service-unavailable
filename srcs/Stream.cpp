#include "../includes/Stream.hpp"

//------------------------------------------------------------------------------
			Stream::Stream()
{}
//------------------------------------------------------------------------------
			Stream::Stream(const Stream& x)
{}
//------------------------------------------------------------------------------
			Stream::~Stream()
{
	while (!buffers.empty())
	{
		delete_buffer();
	}
}
//------------------------------------------------------------------------------
Stream&		Stream::operator=(const Stream& x)
{
	return *this;
}
//------------------------------------------------------------------------------
void		Stream::init(size_t capacity, int in, int out)
{
	fd_in = in;
	fd_out = out;
	default_capacity = capacity;
}
//------------------------------------------------------------------------------
size_t		Stream::fill(size_t s)
{
	if (buffers.empty())
	{
		add_buffer(s);
		it_buffer = buffers.front().start;
	}
	if (buffers.back().remain < s)
	{
		add_buffer(s);
	}
	Buffer&		tmp = buffers.back();
	size_t len = ::read(fd_in, tmp.end, s);
	if (len < 0)
		throw 500;
	tmp.end += len;
	tmp.remain -= len;
	return len;
}
//------------------------------------------------------------------------------
string		Stream::read(size_t s)
{
	string				result;
	uint8_t*			end;

	while (42)
	{
		if (buffers.empty())
			return result;
		end = buffers.front().end;
		while (it_buffer != end && s)
		{
			--s;
			result.push_back(*it_buffer++);
		}
		if (!s)
			return result;
		delete_buffer();
	}
	return result;
}
//------------------------------------------------------------------------------
size_t		Stream::read(size_t s, uint8_t* b)
{
	size_t				len = 0;
	uint8_t*			end;

	while (42)
	{
		if (buffers.empty())
			return len;
		end = buffers.front().end;
		while (it_buffer != end && s)
		{
			*b++ = *it_buffer++;
			++len;
			--s;
		}
		if (!s)
			return len;
		delete_buffer();
	}
	return len;
}
//------------------------------------------------------------------------------
size_t		Stream::pass(size_t s)
{
	uint8_t*	end;
	size_t		len = 0;
	
	if (buffers.empty())
		return 0;
	if (s > end - it_buffer)
	{
		len = ::write(fd_out, it_buffer, end - it_buffer);
		delete_buffer();
	}
	else
		len = ::write(fd_out, it_buffer, s);
	return len;
}
//------------------------------------------------------------------------------
bool		Stream::get_chr_token(string &token, const char c)
{
	uint8_t*	end;
	if (token_factor)
		token.clear();
	while (42)
	{
		if (buffers.empty())
		{
			token_factor = false;
			return false;
		}
		end = buffers.front().end;
		while (it_buffer != end)
		{
			if (c == *it_buffer)
			{
				while (c == *it_buffer)
				{
					++it_buffer;
					if (it_buffer == end)
					{
						delete_buffer();
						if (buffers.empty())
						{
							token_factor = true;
							return true;
						}
						end = buffers.front().end;
					}
				}
				token_factor = true;
				return true;
			}
			token.push_back(*it_buffer);
			++it_buffer;
		}
		delete_buffer();
	}
	token_factor = false;
	return false;
}
//------------------------------------------------------------------------------
bool		Stream::get_line(string &token)
{
	uint8_t*	end;
	if (token_factor)
		token.clear();
	while (42)
	{
		if (buffers.empty())
		{
			token_factor = false;
			return false;
		}
		end = buffers.front().end;
		while (it_buffer != end)
		{
			if ('\n' == *it_buffer)
			{
				++it_buffer;
				if (it_buffer == end)
					delete_buffer();
				if (*--token.end() == '\r')
					token.erase(*--token.end());
				token_factor = true;
				return true;
			}
			token.push_back(*it_buffer);
			++it_buffer;
		}
		delete_buffer();
	}
	token_factor = false;
	return false;
}
//------------------------------------------------------------------------------
void		Stream::add_buffer(size_t s)
{
	buffers.push_back(Buffer());
	s = s > default_capacity ? s : default_capacity;
	Buffer&		tmp = buffers.back();
	tmp.start = new uint8_t[s + 1];
	tmp.end = tmp.start;
	tmp.remain = s;
}
//------------------------------------------------------------------------------
void		Stream::delete_buffer()
{
	delete[] buffers.front().start;
	buffers.pop_front();
	if (!buffers.empty())
		it_buffer = buffers.front().start;
}
//------------------------------------------------------------------------------

/* 
#include <fcntl.h>
#include <unistd.h>

int			main()
{
	int		fd = open("bohemian", O_RDONLY);
	int		len;
	unsigned char	buffer[50];

	Stream	s;
	string	tok;

	cout << fd << endl;
	s.init(6, fd, 1);
	while (s.read(6))
	{
		// cout << s.get(6) << endl;
		usleep(100000);
		if (s.get_chr_token(tok, '\n'))
		{
			cout << tok << endl;
			tok.clear();
		}
		// cout << tok << endl;
	}
} */