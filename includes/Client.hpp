#pragma once
struct Client;
#include <sys/types.h>
#include "Server.hpp"
#include "Enums.hpp"
#include "Socket.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Cgi.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include "Time.hpp"

#define BUFFER_SIZE		16

using std::string;
using std::map;

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
	Time					birth;		// 클라이언트 생성 시각
	Socket					sock;		// 클라이언트의 소켓
	map<string, Server>&	servers;	// 참조할 서버 목록(포트에 종속)
	FdSet&					r_set;
	FdSet&					w_set;
	e_status				status;		// 현재 처리 상태
	HttpReq					req;		// 요청메시지
	HttpRes					res;		// 응답메시지
	Server*					server;		// 참조할 서버
	Location*				location;	// 로케이션
	Method*					method;		// 실행될 메서드

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	private:
					Client();
					Client(const Client&);
	Client&			operator=(const Client&);

	public:
	/*constructor*/	Client(int accept_fd, map<string, Server>& servers, 
					FdSet& r_set, FdSet& w_set);
	virtual			~Client();

	void			routine();
	void			manage_err(int code);

	void			process();
	void				set_request_startline();
	void				set_request_header();
	void				set_client();
	void					set_server();
	void					set_location();
	void					check_auth();
	void					set_path();
	void					set_method();

	void			recv_stream();
	void			send_stream();
	bool			is_expired();	// 생성된 지 너무 오래되면 true

	// void			recv_start_line();
	// void			recv_header();

	// void			set_server();
	// void			set_location();
	// void			translate_path();
	// void			check_auth();
	// void			check_method();


	// void		process_method();





	// void		process_msg();
	// void		process_get();
	// void		process_head();
	// size_t		process_head_base();
	
	// void		send_msg(FdSet& w);
	// void		send_get();
	// void		send_put();
	// void		send_post();
	// void		send_options();
	// void		send_delete();


	// void		recv_body(size_t len);
	// void		recv_body_chunked();
	// void		proc_cgi();
	// char**		make_meta_variable();
	// void		terminate_cgi();

	// void		replace_location();
};	//Client
