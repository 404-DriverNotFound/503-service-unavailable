#include "../includes/Stream.hpp"

//------------------------------------------------------------------------------
			Stream::Stream()
{}
//------------------------------------------------------------------------------
			Stream::Stream(const Stream& x)
{}
//------------------------------------------------------------------------------
			Stream::~Stream()
{}
//------------------------------------------------------------------------------
Stream&		Stream::operator=(const Stream& x)
{}
//------------------------------------------------------------------------------
void		Stream::init(size_t default_capcity, int fd_in, int fd_out)
{
	this->fd_in = fd_in;
	this->fd_out = fd_out;
	buffers.push_back(Buffer());
	buffers.back().start = new uint8_t[default_capacity];
}
//------------------------------------------------------------------------------
void		Stream::fill(size_t s)
{
	Buffer&		buffer_tmp = buffers.back();
	if (s > buffers.back().remain)
	{
		buffers.push_back(Buffer());
		buffer_tmp = buffers.back();
		if (s > default_buffer_size)
			buffer_tmp.start = new uint8_t[s + 1];
		else
			buffer_tmp.start = new uint8_t[default_buffer_size];
		buffer_tmp.remain = s;
		size_t	len = ::read(fd_in, buffer_tmp.start, s);
		if (len < 0)
			throw 500;
		buffer_tmp.remain -= len;
	}
	else
	{
		size_t len = ::read(fd_in, buffer_tmp.end, s);
		if (len < 0)
			throw 500;
		buffer_tmp.remain -= len;
	}
}
#include <fcntl.h>
#include <unistd.h>

//------------------------------------------------------------------------------

int			main()
{
	int		fd = open("Buffer.cpp", O_RDONLY);

	Stream	s;
	s.init()
}