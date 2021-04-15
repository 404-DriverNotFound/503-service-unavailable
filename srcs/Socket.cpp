/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minckim <minckim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:45:05 by minckim           #+#    #+#             */
/*   Updated: 2021/04/15 19:45:25 by minckim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

			Socket::Socket()
{
}

			Socket::~Socket()
{
}

void		Socket::bind(uint16_t port, uint32_t ip = INADDR_ANY)
{
	sockaddr_in&	tmp = reinterpret_cast<sockaddr_in&>(*this);
	fd = socket(PF_INET, SOCK_STREAM, 0);
	ft::memset(this, 0, sizeof(*this));
	socklen = sizeof(sockaddr_in);
	tmp.sin_family = AF_INET;
	tmp.sin_addr.s_addr = ft::hton(ip);
	tmp.sin_port = ft::hton(port);
	::bind(fd, this, socklen);
}

void		Socket::accept(int serv_sock)
{
	fd = ::accept(serv_sock, this, &socklen);
}

void		Socket::listen()
{

}

/*
int			main()
{
	
}
*/