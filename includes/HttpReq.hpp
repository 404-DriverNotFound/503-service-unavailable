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
	uint16_t		method;
	string			path_info;
	list<string>	path;
	string			query;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
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
