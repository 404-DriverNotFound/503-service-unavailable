/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minckim <minckim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:45:05 by minckim           #+#    #+#             */
/*   Updated: 2021/04/15 19:35:43 by minckim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Utils.hpp"

class Socket : public sockaddr
{
	int			fd;
	socklen_t	socklen;

	public:
				Socket();
				Socket(const Socket& x);
	Socket&		operator=(const Socket& x);
	virtual		~Socket();
	void		bind(uint16_t port, uint32_t ip);
	void		accept(int serv_sock);
	void		listen();
};