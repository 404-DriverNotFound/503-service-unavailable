#include "../includes/Stream.hpp"

//------------------------------------------------------------------------------
			Stream::Stream()
{}
//------------------------------------------------------------------------------
			Stream::Stream(size_t default_capacity, int fd_in, int fd_out)
:
pass_remain(0),
fd_in(fd_in),
fd_out(fd_out),
default_capacity(default_capacity),
token_factor(false)
{}
//------------------------------------------------------------------------------
			Stream::Stream(const Stream& x)
{}
//------------------------------------------------------------------------------
			Stream::~Stream()
{
	clear();
}
//------------------------------------------------------------------------------
Stream&		Stream::operator=(const Stream& x)
{
	return *this;
}
//------------------------------------------------------------------------------
void		Stream::init(size_t capacity, int in, int out)
{
	pass_remain = 0;
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
	string::const_iterator	it_string = str.begin();
	string::const_iterator	it_string_end = str.end();
	uint8_t*				end = buffers.back().end;
	
	if (buffers.back().remain >= str.size())
	{
		// 쭉복사
		buffers.back().remain -= str.size();
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		buffers.back().end = end;
	}
	else
	{
		// remain만큼만 복사
		it_string_end = it_string + buffers.back().remain;
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		// 새로 할당해서 나머지 복사
		add_buffer(str.end() - it_string_end);
		end = buffers.back().end;
		it_string_end = str.end();
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		buffers.back().end = end;
	}
}

void		Stream::write(uint8_t*	buff, size_t s)
{
	if (buffers.empty())
	{
		add_buffer(s);
		it_buffer = buffers.front().start;
	}
	uint8_t*	it = buff;
	uint8_t*	it_end = buff + s;
	uint8_t*	end = buffers.back().end;
	
	if (buffers.back().remain >= s)
	{
		// 쭉복사
		buffers.back().remain -= s;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		buffers.back().end = end;
	}
	else
	{
		// remain만큼만 복사
		it_end = it + buffers.back().remain;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		// 새로 할당해서 나머지 복사
		add_buffer(it_end - it_end);
		end = buffers.back().end;
		it_end = it_end;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		buffers.back().end = end;
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
	cout << "enter pass" << len << endl;
	while (s && !buffers.empty())
	{
		end = buffers.front().end;
		
		if (buffers.empty())
		{
			pass_remain = s;
			cout << "empty buffer" << len << endl;
			return 0;
		}
		if (s > end - it_buffer)
		{
			cout << "write1 bf" << len << endl;
			len = ::write(fd_out, it_buffer, end - it_buffer);
			cout << "write1 af" << len << endl;
			delete_buffer();
		}
		else
		{
			cout << "write2 bf" << len << endl;
			len = ::write(fd_out, it_buffer, s);
			cout << "write2 af" << len << endl;
		}
		pass_remain = s - len;
		s -= len;
	}
	cout << "out of pass" << len << endl;
	return len;



	// if (buffers.empty())
	// 	return 0;
	// uint8_t*	end = buffers.front().end;
	// size_t		len = 0;
	
	// if (buffers.empty())
	// {
	// 	pass_remain = s;
	// 	return 0;
	// }
	// if (s > end - it_buffer)
	// {
	// 	len = ::write(fd_out, it_buffer, end - it_buffer);
	// 	delete_buffer();
	// }
	// else
	// 	len = ::write(fd_out, it_buffer, s);
	// pass_remain = s - len;
	// return len;
}
void		Stream::print()
{
	::write(1, it_buffer, buffers.front().end - it_buffer);
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
				{
					token.erase(--token.end());
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
//------------------------------------------------------------------------------
void		Stream::clear()
{
	while (!buffers.empty())
	{
		delete[] buffers.front().start;
		buffers.pop_front();
	}
}
size_t		Stream::size()
{
	list<Buffer>::iterator	it = buffers.begin();
	list<Buffer>::iterator	end = buffers.end();
	size_t					len = 0;
	if (it == end)
		return len;
	len += it->end - it_buffer;
	++it;
	while (it != end)
	{
		len += it->end - it->start;
		++it;
	}
	return len;
}


/* 
#include <fcntl.h>
#include <unistd.h>
int		main()
{
	Stream	s;

	s.init(5, 1, 1);
	s << string("hello\n");
	s << string("how are you\n");
	s.pass();
	s.pass();
}
 */


// #include <fcntl.h>
// #include <unistd.h>

// int			main()
// {
// 	int		fd = open("bohemian", O_RDONLY);
// 	Stream	s;
// 	string	tok;

// 	s.init(5, fd);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);
// 	s.fill(5);

// 	s.pass(20);
// 	usleep(1000000);
// 	s.pass(20);
// 	usleep(1000000);
// 	s.pass(20);
// }


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