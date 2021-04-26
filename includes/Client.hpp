#pragma once
struct Client;
#include <sys/types.h>
#include "Server.hpp"
#include "Stream.hpp"
#include "Socket.hpp"
#include "Buffer.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Cgi.hpp"
#include "FdSet.hpp"
#include "Server.hpp"

#define BUFFER_SIZE		16

using std::string;
using std::map;

enum e_status
{
	RECV_START_LINE,
	RECV_HEADER,
	CHECK_MSG,
	PROC_MSG,
	SEND_MSG,
	SEND_DONE,
};

enum e_status_proc
{
	PROC_CGI,
	END_CGI,
	RECV_BODY,
	RECV_BODY_CHUNKED,
	MAKE_MSG,
};

/*##############################################################################
Client
##############################################################################*/
struct Client
{
	/*--------------------------------------------------------------------------
	Member types
	--------------------------------------------------------------------------*/
	typedef map<string, string>::iterator		iterator_header;
	typedef map<string, string>::iterator		iterator_host;
	typedef map<string, Server>::iterator		iterator_server;
	typedef map<string, Location>::iterator		iterator_location;
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	Socket					sock;
	Stream					stream_in;
	Stream					stream_out;
	string					line;
	e_status				status;
	e_status_proc			status_proc;
	HttpReq					req;
	HttpRes					res;
	Cgi						cgi;
	string					path;
	Server*					server;
	Location*				location;
	string					msg;
	map<string, Server>&	servers;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	private:
				Client();
	public:
				Client(const Client&);
				Client(map<string, Server>& servers);
	virtual		~Client();
	Client&		operator=(const Client&);
	void		init(int fd);

	void		client_process(FdSet& r, FdSet& w);
	
	void		recv_start_line();
	void		recv_header();

	void		set_location();
	void		check_auth();
	void		check_method();

	void		process_msg();
	void		process_get();
	void		process_put();
	void		process_post();
	void		process_options();
	void		process_delete();
	
	void		send_msg(FdSet& w);
	void		send_get();
	void		send_put();
	void		send_post();
	void		send_options();
	void		send_delete();


	void		recv_body(size_t len);
	void		recv_body_chunked();
	void		proc_cgi();
	char**		make_meta_variable();
	void		terminate_cgi();

	void		replace_location();
};	//Client
