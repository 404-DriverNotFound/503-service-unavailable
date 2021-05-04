#pragma once
struct HttpReq;
#include "HttpRes.hpp"
#include "Http.hpp"

/*##############################################################################
Http Request
##############################################################################*/

using std::map;
using std::string;
using std::list;

struct HttpReq : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	string			line;
	string			path_translated;
	string			path_info;
	string			extension;
	string			query;
	uint32_t		method;
	list<string>	path;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	/*constructor*/	HttpReq(int sock_fd);
	void			set_start_line(string& line);
	void			set_header(string& line);
	void			set_path(string& token);
	string			get_location_name();
	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void			set_method(const string& token);
};	// Http Request
