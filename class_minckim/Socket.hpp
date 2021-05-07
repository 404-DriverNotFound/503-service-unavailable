/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elegie <elegie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:45:05 by minckim           #+#    #+#             */
/*   Updated: 2021/05/07 02:36:12 by elegie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Utils.hpp"

struct Socket : public sockaddr
{

	public:
	int			fd;
	socklen_t	socklen;
				Socket(uint32_t port, uint32_t ip);
				Socket(int fd);
				Socket(const Socket& x);
	Socket&		operator=(const Socket& x);
	virtual		~Socket();
	void		bind(uint32_t port, uint32_t ip);
	void		accept(int serv_sock);
	void		listen(size_t connections);

	// exceptions
	class socket_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class bind_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class accept_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class listen_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
};