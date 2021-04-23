#include "Buffer.hpp"

Buffer::Buffer()
: buffer(0)
{}

//------------------------------------------------------------------------------

					Buffer::Buffer(int fd, size_t buffer_size)
: fd(fd), buffer_size(buffer_size)
{
	read_request = 1;
	buffer = new char[buffer_size + 1];
	cursor = buffer;
	end = buffer;
}

//------------------------------------------------------------------------------

					Buffer::Buffer(const Buffer& x)
:fd(x.fd),
buffer_size(x.buffer_size),
buffer(x.buffer),
cursor(x.cursor),
end(x.end),
read_request(x.read_request),
is_token_complete(x.is_token_complete),
write_request(x.write_request),
len(x.len)
{}

//------------------------------------------------------------------------------

Buffer&				Buffer::operator=(const Buffer& x)
{
	if (&x == this)
		return *this;
	fd = x.fd;
	buffer_size = x.buffer_size;
	buffer = x.buffer;
	cursor = x.cursor;
	end = x.end;
	read_request = x.read_request;
	is_token_complete = x.is_token_complete;
	write_request = x.write_request;
	len = x.len;
	return *this;
}

//------------------------------------------------------------------------------

					Buffer::~Buffer()
{
	if (buffer)
		delete[] buffer;
}

//------------------------------------------------------------------------------

int					Buffer::read_buffer()
{

	if (!read_request)
		return 0;
	len = -1;
	if ((len = read(fd, buffer, buffer_size)) < 0)
		return len;
	end = buffer + len;
	cursor = buffer;
	read_request = 0; // off
	return len;

}

//------------------------------------------------------------------------------

void				Buffer::get_token(std::string& token, int sep)
{
	if (read_request)
		return ;
	if (is_token_complete)
		token.clear();
	while (cursor != end)
	{
		if (sep == *cursor)
		{
			cursor++;
			is_token_complete = 1;
			return ;
		}
		token.push_back(*cursor++);
	}
	is_token_complete = 0;
	read_request = 1; // on
}

//------------------------------------------------------------------------------

void				Buffer::get_token_seq(std::string& token, char* seq)
{
	size_t			len_seq = ft::strlen(seq);

	if (read_request)
		return ;
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
	is_token_complete = 0;
	read_request = 1; // on
}

//------------------------------------------------------------------------------

char				Buffer::get_token_set(std::string& token, char* set)
{
	char*			seperator;

	if (read_request)
		return 0;
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
	is_token_complete = 0;
	read_request = 1; // on
	return 0;
}

//------------------------------------------------------------------------------

ssize_t				Buffer::size() const
{
	return buffer - end;
}

//------------------------------------------------------------------------------

void				Buffer::write(size_t s, int fd)
{
	if (static_cast<size_t>(end - cursor) > s)
	{
		::write(fd, cursor, s);
		write_request = 0;
	}
	else
	{
		::write(fd, cursor, (end - cursor));
		write_request = s - (end - cursor);
		read_request = 1;
	}
}


/*
// tester
#include <fcntl.h>
int		main()
{

	int		fd = open("Buffer.hpp", O_RDONLY);

	Buffer	buffer(fd, 4);

	std::string	token;


	while (1)
	{
		if (buffer.read_request)
			buffer.read_buffer();
		if (buffer.len == 0)
			break ;
		buffer.get_token_set(token, ";\n");
		if (buffer.is_token_complete)
			std::cout << token << std::endl;
	}



}
*/