#include "../includes/Buffer.hpp"

Buffer::Buffer()
:fd_in(-1),
fd_out(-1),
buffer_size(0),
buffer(0),
cursor(0),
end(0),
is_readable(0),
is_token_complete(0),
len(0)
{}

//------------------------------------------------------------------------------

					Buffer::Buffer(int fd_in, size_t buffer_size)
: fd_in(fd_in), buffer_size(buffer_size)
{
	buffer = new char[buffer_size + 1];
	cursor = buffer;
	end = buffer;
}

//------------------------------------------------------------------------------

					Buffer::Buffer(const Buffer& x)
:fd_in(-1),
fd_out(-1),
buffer_size(0),
buffer(0),
cursor(0),
end(0),
is_readable(0),
is_token_complete(0),
len(0)
{}

//------------------------------------------------------------------------------

Buffer&				Buffer::operator=(const Buffer& x)
{
	if (&x == this)
		return *this;
	fd_in = -1;
	fd_out = -1;
	buffer_size = 0;
	buffer = 0;
	cursor = 0;
	end = 0;
	is_readable = 0;
	is_token_complete = 0;
	len = 0;
	return *this;
}

//------------------------------------------------------------------------------

					Buffer::~Buffer()
{
	if (buffer)
		delete[] buffer;
}

//------------------------------------------------------------------------------

void				Buffer::init(int fd_in, size_t buffer_size)
{
	if (buffer)
		delete[] buffer;
	this->fd_in = fd_in;
	this->buffer_size = buffer_size;
	buffer = new char[buffer_size + 1];
	cursor = buffer;
	end = buffer;
}

//------------------------------------------------------------------------------

size_t				Buffer::refill_buffer()
{
	cout << __func__ << endl;
	cout << "	is_readable: " << is_readable << endl;
	if (is_readable == false)
		return 0;
	is_readable = false;
	if ((len = read(fd_in, buffer, buffer_size)) < 0)
		return len;
	cout << "	len: " << len << endl;
	end = buffer + len;
	cursor = buffer;
	return len;

}

//------------------------------------------------------------------------------

void				Buffer::get_token(std::string& token, int sep)
{
	if (is_token_complete == true)
		token.clear();
	while (cursor != end)
	{
		if (sep == *cursor)
		{
			cursor++;
			is_token_complete = true;
			return ;
		}
		token.push_back(*cursor++);
	}
	is_token_complete = false;
	if (is_readable == true)
	{
		refill_buffer();
		get_token(token, sep);
	}
}

//------------------------------------------------------------------------------

void				Buffer::get_token_seq(std::string& token, char* seq)
{
	size_t			len_seq = ft::strlen(seq);

	if (is_token_complete)
		token.clear();
	while (cursor != end)
	{
		token.push_back(*cursor++);
		if (len_seq <= token.length() && ft::strcmp(&*token.end() - len_seq, seq) == 0)
		{
			token.erase(token.end() - len_seq, token.end());
			is_token_complete = 1;
			return ;
		}
	}
	if (is_readable == true)
	{
		refill_buffer();
		while (cursor != end)
		{
			token.push_back(*cursor++);
			if (len_seq <= token.length() && ft::strcmp(&*token.end() - len_seq, seq) == 0)
			{
				token.erase(token.end() - len_seq, token.end());
				is_token_complete = 1;
				return ;
			}
		}		
	}
	is_token_complete = false;
}

//------------------------------------------------------------------------------

char				Buffer::get_token_set(std::string& token, char* set)
{
	char*			seperator;

	if (is_token_complete)
		token.clear();
	while (cursor != end)
	{
		if ((seperator = ft::strchr(set, *cursor)))
		{
			cursor++;
			is_token_complete = 1;
			return *seperator;
		}
		token.push_back(*cursor++);
	}
	if (is_readable == true)
	{
		refill_buffer();
		while (cursor != end)
		{
			if ((seperator = ft::strchr(set, *cursor)))
			{
				cursor++;
				is_token_complete = 1;
				return *seperator;
			}
			token.push_back(*cursor++);
		}		
	}
	is_token_complete = false;
	return 0;
}

//------------------------------------------------------------------------------

ssize_t				Buffer::size() const
{
	return buffer - end;
}

//------------------------------------------------------------------------------

size_t				Buffer::write(size_t s)
{
	size_t		written = 0;
	if (is_writeable == false)
		return 0;
	if (static_cast<size_t>(end - cursor) > s)
	{
		written = ::write(fd_out, cursor, s);
		cursor += written;
		is_writeable = false;
	}
	else
	{
		written = ::write(fd_out, cursor, (end - cursor));
		cursor += written;
		s -= written;
		if (is_readable && cursor == end)
		{
			refill_buffer();
			written += write(s);
		}
	}
	return written;
}

//------------------------------------------------------------------------------

bool				Buffer::is_read_req()
{
	return !(end - cursor);
}

//------------------------------------------------------------------------------
/* 
#include <fcntl.h>
using std::cout;
using std::endl;
int			main()
{
	int		fd = open("test", O_RDONLY);
	int		status = 0;

	Buffer	b;
	string	tok;

	fd_set	o;
	fd_set	r;
	fd_set	w;
	timeval	time = {1, 0};

	FD_ZERO(&o);
	FD_SET(fd, &o);
	FD_SET(1, &o);
	r = o;

	int		len;

	b.init(fd, 5);
	b.fd_out = 1;
	fcntl(fd, O_NONBLOCK);
	while (1)
	{
		usleep(100000);
		r = o;
		w = o;
		time.tv_sec = 5;
		time.tv_usec = 0;
		if (select(100, &r, &w, 0, &time) == 0)
		{
			cout << "continue\n";
			continue;
		}
		if FD_ISSET(fd, &r)
			b.is_readable = true;
		if FD_ISSET(1, &w)
			b.is_writeable = true;
		if (status == 0)
		{
			b.get_token(tok, '\n');
			if (b.is_token_complete == false)
				continue;
			len = atoi(tok.c_str());
			status = 1;
		}
		if (status == 1)
		{
			len -= b.write(len);
			if (len == 0)
				status = 0;
		}
	}
} */