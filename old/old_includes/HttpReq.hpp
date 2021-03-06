#pragma once
#include "Http.hpp"
#include "Utils.hpp"

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
	size_t			recv_length;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	/*constructor*/	HttpReq(int sock_fd);
	void			set_start_line(string& line);
	void			set_path(string& token);
	string			get_location_name();
	void			clear();
	void			set_extension();
	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void			set_method(const string& token);
};	// Http Request
