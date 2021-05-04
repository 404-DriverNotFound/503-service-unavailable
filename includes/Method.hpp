#pragma once
#include <map>
#include <string>
#include "Utils.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"

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
	METHOD_START_CGI,
	METHOD_CGI_IS_RUNNING,
	METHOD_READ_CGI_HEAD,
	METHOD_LOAD_HEADER,
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
	static map<string, uint16_t>	method_num;
	static map<string, uint16_t>	method_flags;
	static string					method_strings[NUM_METHOD];

	static string&					get_method(uint16_t num);
	static uint16_t					get_method(const string& method_name);
	static string					get_allow(uint16_t flag);

	static void						init_method_num();
	static void						init_method_flags();
	static void						init_method_strings();
	HttpReq&						req;
	HttpRes&						res;
	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
	/*constructor*/					Method();
	/*constructor*/					Method(const Method& x);
	/*constructor*/					Method();
	/*destrucctor*/					~Method();
	Method&							operator=(const Method& x);
	virtual bool					run() = 0;


};