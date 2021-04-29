/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elegie <elegie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:45:05 by minckim           #+#    #+#             */
/*   Updated: 2021/04/30 00:45:47 by elegie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Utils.hpp"

using std::cout;
using std::endl;

/*##############################################################################
Socket
##############################################################################*/

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
	public:
				Socket();
				Socket(const Socket& x);
	Socket&		operator=(const Socket& x);
				Socket(int fd);							// accept
				Socket(uint16_t port, uint32_t ip);		// bind
	virtual		~Socket();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void		bind(uint16_t port, uint32_t ip);
	void		accept(int serv_sock);
	void		listen(size_t connections);

	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	class socket_failed_exception : public std::exception{
		virtual const char*	what() const throw();	
	};
	class bind_failed_exception : public std::exception{
		virtual const char*	what() const throw();	
	};
	class accept_failed_exception : public std::exception{
		virtual const char*	what() const throw();	
	};
	class listen_failed_exception : public std::exception{
		virtual const char*	what() const throw();	
	};
};
