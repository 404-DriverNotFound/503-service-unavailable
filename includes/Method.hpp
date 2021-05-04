#pragma once
#include <map>
#include <string>
#include "Utils.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Cgi.hpp"

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
	HttpReq&						req;
	HttpRes&						res;
	Cgi*							cgi;
	int								fd_in;
	int								fd_out;
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
	/*constructor*/					Method(HttpReq& req, HttpRes& res);
	/*destrucctor*/					~Method();

	bool							is_cgi_extension();
	void							recv_init();
	bool							recv_body();
	bool							recv_chunked_body();
	void							run_cgi();
	void							set_cgi_header();
	virtual void					load_response_header();
	void							load_cgi_tmp_remain();
	void							load_body();
	char**							make_meta_variable();

	void							open_in_file();
	void							open_out_file();

	virtual bool					run() = 0;

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