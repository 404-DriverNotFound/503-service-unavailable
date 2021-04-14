#pragma once
class Client;
#include "Socket.hpp"
#include "Buffer.hpp"
#include "Http.hpp"
#include <sys/types.h>
#include "Config.hpp"

#define BUFFER_SIZE		16

using std::string;

enum e_status
{
	RECV_START_LINE,
	RECV_HEADER,
	PROC_CGI,
	END_CGI,
	RECV_BODY,
	MAKE_MSG,
	SEND_MSG,
	SEND_DONE
};

class Client
{
	Socket		sock;
	Buffer		buffer;
	std::string	line;
	e_status	status;
	Http		req;
	Http		res;
	Config&		config_location;

	public:
	void		manage_client(bool is_buffer);
	void		read_buffer();
	void		recv_start_line();
	void		recv_header();
	void		recv_body();
	void		proc_cgi();
	void		terminate_cgi();
	void		make_msg();
	void		send_msg();
};