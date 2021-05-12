#include "../includes/Stream.hpp"

// #define DBG

//------------------------------------------------------------------------------
			Stream::Stream()
{}
//------------------------------------------------------------------------------
			Stream::Stream(size_t default_capacity, int fd_in, int fd_out)
:
_pass_remain(0),
_fd_in(fd_in),
_fd_out(fd_out),
_default_capacity(default_capacity),
_token_factor(false)
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
	_pass_remain = 0;
	_fd_in = in;
	_fd_out = out;
	_default_capacity = capacity;
	_token_factor = false;
}
//------------------------------------------------------------------------------
ssize_t		Stream::fill(size_t s)
{
	// cout << __func__ << endl;
	if (_buffers.empty())
	{
		add_buffer(s);
		_it_buffer = _buffers.front().start;
	}
	if (_buffers.back().remain < s)
	{
		add_buffer(s);
	}
	Buffer&		tmp = _buffers.back();
	ssize_t len = ::read(_fd_in, tmp.end, s);
	if (len < 0)
		return -1;
	// if (len < 0)
	// 	throw 500;
	tmp.end += len;
	tmp.remain -= len;
	return len;
}
//------------------------------------------------------------------------------
void		Stream::write(const string& str)
{
	if (_buffers.empty())
	{
		add_buffer(str.size());
		_it_buffer = _buffers.front().start;
	}
	string::const_iterator	it_string = str.begin();
	string::const_iterator	it_string_end = str.end();
	uint8_t*				end = _buffers.back().end;
	
	if (_buffers.back().remain >= str.size())
	{
		// 쭉복사
		_buffers.back().remain -= str.size();
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		_buffers.back().end = end;
	}
	else
	{
		// remain만큼만 복사
		it_string_end = it_string + _buffers.back().remain;
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		// 새로 할당해서 나머지 복사
		add_buffer(str.end() - it_string_end);
		end = _buffers.back().end;
		it_string_end = str.end();
		while (it_string != it_string_end)
		{
			*end++ = *it_string;
			++it_string;
		}
		_buffers.back().end = end;
	}
}

void		Stream::write(uint8_t*	buff, size_t s)
{
	if (_buffers.empty())
	{
		add_buffer(s);
		_it_buffer = _buffers.front().start;
	}
	uint8_t*	it = buff;
	uint8_t*	it_end = buff + s;
	uint8_t*	end = _buffers.back().end;
	
	if (_buffers.back().remain >= s)
	{
		// 쭉복사
		_buffers.back().remain -= s;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		_buffers.back().end = end;
	}
	else
	{
		// remain만큼만 복사
		it_end = it + _buffers.back().remain;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		// 새로 할당해서 나머지 복사
		add_buffer(it_end - it_end);
		end = _buffers.back().end;
		it_end = it_end;
		while (it != it_end)
		{
			*end++ = *it++;
		}
		_buffers.back().end = end;
	}
}
//------------------------------------------------------------------------------
string		Stream::read(size_t s)
{
	string				result;
	uint8_t*			end;

	while (42)
	{
		if (_buffers.empty())
			return result;
		end = _buffers.front().end;
		while (_it_buffer != end && s)
		{
			--s;
			result.push_back(*_it_buffer++);
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
		if (_buffers.empty())
			return len;
		end = _buffers.front().end;
		while (_it_buffer != end && s)
		{
			*b++ = *_it_buffer++;
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
	if (_buffers.empty())
		return 0;

	uint8_t*	end = _buffers.front().end;
	size_t		len = 0;
	while (s && !_buffers.empty())
	{
		end = _buffers.front().end;
		
		if (_buffers.empty())
		{
			_pass_remain = s;
			// cout << "  - empty buffer" << len << endl;
			return 0;
		}
		if (s > end - _it_buffer)
		{
			// cout << "  - write1 bf: " << len << endl;
			len = ::write(_fd_out, _it_buffer, end - _it_buffer);
			// cout << "  - write1 af: " << len << endl;
			delete_buffer();
		}
		else
		{
			// cout << "  - write2 bf: " << len << endl;
			len = ::write(_fd_out, _it_buffer, s);
			// cout << "  - write2 af: " << len << endl;
		}
		_pass_remain = s - len;
		s -= len;
	}
	// cout << "out of pass" << len << endl;
	return len;
}

//------------------------------------------------------------------------------

void		Stream::print()
{
	list<Buffer>::iterator it = _buffers.begin();
	list<Buffer>::iterator end = _buffers.end();
	while (it != end)
	{
		::write(1, it->start, it->end - _it_buffer);
		++it;
	}
}
void		Stream::print_line()
{
	::write(1, _it_buffer,  _buffers.front().end - _it_buffer);
}

//------------------------------------------------------------------------------
size_t		Stream::pass()
{
	uint8_t*	end = _buffers.front().end;
	size_t		len = 0;
	
	if (_buffers.empty())
		return 0;
	len = ::write(_fd_out, _it_buffer, end - _it_buffer);
	if (len != end - _it_buffer)
	{
		_it_buffer += len;
	}
	else
	{
		delete_buffer();
	}
	return len;
}
//------------------------------------------------------------------------------
bool		Stream::get_chr_token(string &token, const char c)
{
	uint8_t*	end;
	if (_token_factor)
		token.clear();
	while (42)
	{
		if (_buffers.empty())
		{
			_token_factor = false;
			return false;
		}
		end = _buffers.front().end;
		while (_it_buffer != end)
		{
			if (c == *_it_buffer)
			{
				while (c == *_it_buffer)
				{
					++_it_buffer;
					if (_it_buffer == end)
					{
						delete_buffer();
						if (_buffers.empty())
						{
							_token_factor = true;
							return true;
						}
						end = _buffers.front().end;
					}
				}
				_token_factor = true;
				return true;
			}
			token.push_back(*_it_buffer);
			++_it_buffer;
		}
		delete_buffer();
	}
	_token_factor = false;
	return false;
}

//------------------------------------------------------------------------------

bool		Stream::get_seq_token(string &token, const char* seq)
{
	uint8_t*	end;
	size_t		len_seq = ft::strlen(seq);
	if (_token_factor)
		token.clear();
	while (42)
	{
		if (_buffers.empty())
		{
			_token_factor = false;
			return false;
		}
		end = _buffers.front().end;
		while (_it_buffer != end)
		{
			token.push_back(*_it_buffer);
			++_it_buffer;
			if (token.size() >= len_seq)
			{
				if (ft::strcmp((token.end() - len_seq).base(), seq) == 0)
				{
					return true;
				}
			}
		}
		delete_buffer();
	}
	_token_factor = false;
	return false;
}

//------------------------------------------------------------------------------
bool		Stream::get_line(string &token)
{
	uint8_t*	end;
	if (_token_factor)
		token.clear();
	while (42)
	{
		if (_buffers.empty())
		{
			_token_factor = false;
			return false;
		}
		end = _buffers.front().end;
		while (_it_buffer != end)
		{
			if ('\n' == *_it_buffer)
			{
				++_it_buffer;
				if (_it_buffer == end)
					delete_buffer();
				if (*--token.end() == '\r')
				{
					token.erase(--token.end());
				}
				_token_factor = true;
				return true;
			}
			token.push_back(*_it_buffer);
			++_it_buffer;
		}
		delete_buffer();
	}
	_token_factor = false;
	return false;
}
//------------------------------------------------------------------------------
void		Stream::add_buffer(size_t s)
{
	_buffers.push_back(Buffer());
	s = s > _default_capacity ? s : _default_capacity;
	Buffer&		tmp = _buffers.back();
	tmp.start = new uint8_t[s + 1];
	tmp.end = tmp.start;
	tmp.remain = s;
}
//------------------------------------------------------------------------------
void		Stream::delete_buffer()
{
	delete[] _buffers.front().start;
	_buffers.pop_front();
	if (!_buffers.empty())
		_it_buffer = _buffers.front().start;
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
	while (!_buffers.empty())
	{
		delete[] _buffers.front().start;
		_buffers.pop_front();
	}
}
size_t		Stream::size()
{
	list<Buffer>::iterator	it = _buffers.begin();
	list<Buffer>::iterator	end = _buffers.end();
	size_t					len = 0;
	if (it == end)
		return len;
	len += it->end - _it_buffer;
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


// int			main()
// {
// 	Stream	s(6, 1, 1);

// 	s << "abcdef01234";

// 	string	tok;
// 	cout << s.get_seq_token(tok, "ef0") << endl;
// 	cout << tok << endl;
// 	cout << ft::strlen("ef0") << endl;
// }