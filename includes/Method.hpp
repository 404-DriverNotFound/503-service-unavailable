#pragma once
#include <map>
#include <string>
#include "Utils.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Cgi.hpp"
#include "Server.hpp"
#include "Location.hpp"

enum e_method
{
	CONNECT = 0,
	DELETE,
	GET,
	HEAD,
	OPTIONS,
	PATCH,
	POST,
	PUT,
	TRACE,
	NUM_METHOD
};


enum e_method_status
{
	METHOD_RECV_BODY,
	METHOD_RECV_CHUNKED_BODY,
	METHOD_START_CGI,
	METHOD_CGI_IS_RUNNING,
	METHOD_LOAD_HEADER_CGI,
	METHOD_LOAD_HEADER,
	METHOD_LOAD_BODY,
	METHOD_DONE,
};

enum e_method_status_chunked
{
	CHUNKED_SIZE,
	CHUNKED_RECV,
	CHUNKED_NL,
};

enum e_openfile
{
	OPEN_PUT,
	OPEN_GET,
	OPEN_POST,
	OPEN_POST_CGI,
	// DELETE,
};

using std::cout;
using std::endl;
using std::string;
using std::map;

/*##############################################################################
Method
##############################################################################*/

struct	Method
{
	/*--------------------------------------------------------------------------
	Members
	--------------------------------------------------------------------------*/
	Server&							server;
	Location&						location;
	HttpReq&						req;
	HttpRes&						res;
	Cgi*							cgi;
	int								fd_in;
	int								fd_out;
	string							name_in;
	string							name_out;
	e_method_status					status;
	e_method_status_chunked			status_chunked;

	/*--------------------------------------------------------------------------
	Static Members
	--------------------------------------------------------------------------*/
	static map<string, uint32_t>	method_num;
	static map<string, uint32_t>	method_flags;
	static string					method_strings[NUM_METHOD];

	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
	private:
	/*constructor*/					Method();
	/*constructor*/					Method(const Method& x);
	Method&							operator=(const Method& x);

	public:
	/*constructor*/					Method(HttpReq& req, HttpRes& res, Server& server, Location& location);
	/*destrucctor*/					~Method();

	bool							is_cgi_extension(); // cgi를 실행하는 확장자이면 참
	void							recv_init();		// 본문 받기 초기화
	bool							recv_body();		// 본문 받기, 끝나면 참
	bool							recv_chunked_body();	// 조각난 본문 받기, 끝나면 참
	void							run_cgi();			// cgi 실행
	void							set_cgi_header();	// cgi 실행결과에서 헤드 추가하기
	virtual void					load_response_header();	// 응답헤더를 스트림에 적재
	void							load_cgi_tmp_remain();	// 응답헤더를 만들고 남은 것을 스트림에 적재
	void							load_body();		// cgi실행결과를 적재
	char**							make_meta_variable();	// 메타변수 생성

	void							open_file_base(const string& path);
	virtual void					open_file_in(const string& name);	// 본문을 받아들이는 파일
	virtual void					open_file_out(const string& name);	// 전송할 파일
	void							open_file(e_openfile option);	// 전송할 파일
	string							temp_name();
	virtual bool					run();

	/*--------------------------------------------------------------------------
	Static Methods
	--------------------------------------------------------------------------*/
	static string&					get_method(uint32_t num);
	static uint32_t					get_method(const string& method_name);
	static string					get_allow(uint32_t flag);

	static void						init_method_num();
	static void						init_method_flags();
	static void						init_method_strings();
};
