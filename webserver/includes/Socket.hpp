#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
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
		Socket();
		Socket(const Socket& x);
		Socket&		operator=(const Socket& x);
	public:
		Socket(int fd);							// accept
		Socket(uint32_t port, uint32_t ip);		// bind
		virtual		~Socket();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
		void		bind(uint32_t port, uint32_t ip);
		void		accept(int serv_sock);
		void		listen(size_t connections);

	/*==========================================================================
	getter
	==========================================================================*/
	int			get_fd();
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