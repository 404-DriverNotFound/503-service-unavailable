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
	token_factor = false;
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
void		Stream::write(const string& str)
{
	if (buffers.empty())
	{
		add_buffer(str.size());
		it_buffer = buffers.front().start;
	}
	string::const_iterator	it = str.begin();
	string::const_iterator	it_end = str.end();
	uint8_t*				end = buffers.back().end;
	size_t					remain;
	
	if (buffers.back().remain > str.size())
	{
		// 쭉복사
		buffers.back().remain -= it_end - it;
		while (it != it_end)
		{
			*end++ = *it;
			++it;
		}
	}
	else
	{
		// remain만큼만 복사
		it_end = it + buffers.back().remain;
		while (it != it_end)
		{
			*end++ = *it;
			++it;
		}
		// 새로 할당해서 나머지 복사
		add_buffer(str.end() - it_end);
		end = buffers.back().end;
		it_end = str.end();
		while (it != it_end)
		{
			*end++ = *it;
			++it;
		}
	}
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
	if (buffers.empty())
		return 0;
	uint8_t*	end = buffers.front().end;
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
size_t		Stream::pass()
{
	uint8_t*	end = buffers.front().end;
	size_t		len = 0;
	
	if (buffers.empty())
		return 0;
	len = ::write(fd_out, it_buffer, end - it_buffer);
	delete_buffer();
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
					token.erase(--token.end());
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
Stream&		Stream::operator<<(const string& str)
{
	write(str);
	return *this;
}

/* 
#include <fcntl.h>
#include <unistd.h>

int			main()
{
	int		fd = open("bohemian", O_RDONLY);
	Stream	s;
	string	tok;

	s.init(5, fd);
	s.fill(10);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(10);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	s.fill(5);
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	cout << s.get_line(tok) << "|"
	<< tok << endl;
	cout << s.get_line(tok) << "|"
	<< tok << endl;

} */



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