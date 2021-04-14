#include "Buffer.hpp"

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
/*
* param
* std::string&		token
* int				sep
*/
int					Buffer::get_token(std::string& token, int sep)
{
	if (flag_token == 1)
		token.clear();
	while (cursor != end)
	{
		if (sep == *cursor)
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

//------------------------------------------------------------------------------
int					Buffer::get_token(std::deque<uint8_t>& token, int sep)
{
	if (flag_token == 1)
		token.clear();
	while (cursor != end)
	{
		if (sep == *cursor)
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

//------------------------------------------------------------------------------
ssize_t				Buffer::size() const
{
	return buffer - end;
}

//------------------------------------------------------------------------------
// param:
// length to read
// return:
// remain length
ssize_t				Buffer::write(size_t s)
{
	if ((end - cursor) > s)
	{
		::write(1, cursor, s);
		return 0;
	}
	else
	{
		::write(1, cursor, (end - cursor));
		return s - (end - cursor);
	}


}

int		main()
{}