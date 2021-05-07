#include "../includes/Socket.hpp"
#include <stdio.h>
//------------------------------------------------------------------------------

			Socket::Socket()
: fd(-1)
{
	throw 10000;
}

//------------------------------------------------------------------------------

			Socket::Socket(uint32_t port, uint32_t ip)
: s_addr()
{
	bind(port, ip);
}

//------------------------------------------------------------------------------

			Socket::Socket(int fd)
: s_addr()
{
	accept(fd);
}

//------------------------------------------------------------------------------

			Socket::Socket(const Socket& x)
: fd(-1)
{
	throw 10000;
}

//------------------------------------------------------------------------------

Socket&		Socket::operator=(const Socket& x)
{
	throw 10000;
	fd = -1;
	return *this;
}

//------------------------------------------------------------------------------

			Socket::~Socket()
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		// close(fd);
	}
}

//------------------------------------------------------------------------------

void		Socket::bind(uint32_t port, uint32_t ip)
{
	ft::memset(&s_addr, 0, sizeof(s_addr));
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		throw socket_failed_exception();
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	if (::bind(fd, reinterpret_cast<sockaddr*>(&s_addr), sizeof(s_addr)))
		throw bind_failed_exception();
}

//------------------------------------------------------------------------------

void		Socket::accept(int serv_sock)
{
	if ((fd = ::accept(serv_sock, reinterpret_cast<sockaddr*>(&s_addr), &socklen)) < 0)
		throw accept_failed_exception();
	fcntl(fd, F_SETFL, O_NONBLOCK);
	linger	lin;
	lin.l_onoff = 0;
	lin.l_linger = 1;
	setsockopt(fd, SOL_SOCKET, SO_LINGER, &lin, sizeof(linger));
}

//------------------------------------------------------------------------------

void		Socket::listen(size_t connections)
{
	if (::listen(fd, connections) < 0)
		throw listen_failed_exception();
}

/*--------------------------------------------------------------------------
Exception
--------------------------------------------------------------------------*/

const char*	Socket::socket_failed_exception::what() const throw()
{ return "Socket failed"; }

const char*	Socket::bind_failed_exception::what() const throw()
{	
	printf("errno: %d\n", errno);
	perror("result : ");
	return "bind failed"; }

const char*	Socket::accept_failed_exception::what() const throw()
{ return "accept failed"; }

const char*	Socket::listen_failed_exception::what() const throw()
{
	printf("errno: %d\n", errno);
	perror("result : ");
	return "listen failed"; }

