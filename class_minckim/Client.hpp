#pragma once
class Client;
#include <sys/types.h>
#include "Socket.hpp"
#include "Buffer.hpp"
#include "Http.hpp"
#include "Config.hpp"
#include "Cgi.hpp"
#include "FdSet.hpp"

#define BUFFER_SIZE		16

using std::string;

enum e_status
{
	RECV_START_LINE,
	RECV_HEADER,
	PROC_CGI,
	END_CGI,
	RECV_BODY,
	RECV_BODY_CHUNKED,
	MAKE_MSG,
	SEND_MSG,
	SEND_DONE
};

class Client
{
	public:
	Socket		sock;
	Buffer		buffer;
	std::string	line;
	e_status	status;
	HttpReq		req;
	HttpRes		res;
	Cgi			cgi;
	std::string	path;
	Config&		config_location;

				Client(int fd);
	void		client_process(FdSet& r, FdSet& w);
	void		read_buffer();
	void		recv_start_line();
	void		recv_header();
	void		recv_body(size_t len);
	void		recv_chunked_body();
	void		proc_cgi();
	void		terminate_cgi();
	void		make_msg();
	void		send_msg();
};