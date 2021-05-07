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
#include <errno.h>
#include "Utils.hpp"

/*##############################################################################
Socket
##############################################################################*/

struct bind_type{};

struct Socket
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	int			fd;
	sockaddr_in	s_addr;
	socklen_t	socklen;

	/*--------------------------------------------------------------------------
	Canonical Form
	--------------------------------------------------------------------------*/
	private:
	Socket&		operator=(const Socket& x);
	public:
				Socket();
				// Socket(const Socket& x);
				Socket(int fd);							// accept
				Socket(uint32_t port, uint32_t ip);		// bind
	virtual		~Socket();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void		bind(uint32_t port, uint32_t ip);
	void		accept(int serv_sock);
	void		listen(size_t connections);

	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	class socket_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class bind_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class accept_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
	class listen_failed_exception : public std::exception{
		virtual const char*	what() const throw();	};
};
