/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elegie <elegie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:45:05 by minckim           #+#    #+#             */
/*   Updated: 2021/04/17 08:42:39 by elegie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

			Socket::Socket(uint16_t port, uint32_t ip = INADDR_ANY)
{
	bind(port, ip);
}

			Socket::Socket(int fd)
{
	accept(fd);
}

			Socket::~Socket()
{
	close(fd);
}

void		Socket::bind(uint16_t port, uint32_t ip = INADDR_ANY)
{
	sockaddr_in&	tmp = reinterpret_cast<sockaddr_in&>\
							(dynamic_cast<sockaddr&>(*this));
	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		throw socket_failed_exception();
	ft::memset(this, 0, sizeof(*this));
	socklen = sizeof(sockaddr_in);
	tmp.sin_family = AF_INET;
	tmp.sin_addr.s_addr = ft::hton(ip);
	tmp.sin_port = ft::hton(port);
	if (::bind(fd, this, socklen))
		throw bind_failed_exception();
}

void		Socket::accept(int serv_sock)
{
	if ((fd = ::accept(serv_sock, this, &socklen)))
		throw accept_failed_exception();
}

void		Socket::listen(size_t connections)
{
	if (::listen(fd, connections) < 0)
		throw listen_failed_exception();
}

//------------------------------------------------------------------------------
// exceptions
const char*	Socket::socket_failed_exception::what() const throw()
{ return "Socket failed"; }

const char*	Socket::bind_failed_exception::what() const throw()
{ return "bind failed"; }

const char*	Socket::accept_failed_exception::what() const throw()
{ return "accept failed"; }

const char*	Socket::listen_failed_exception::what() const throw()
{ return "listen failed"; }

