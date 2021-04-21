#pragma once
struct HttpReq;
#include "HttpRes.hpp"
#include "Http.hpp"

/*##############################################################################
Http Request
##############################################################################*/
struct HttpReq : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	uint16_t		method;
	std::string		path;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void			set_start_line(const std::string& line);
	void			set_headers(const std::string& line);
	void			get_location_name(std::string& location_name);
	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void			set_method(const std::string& token);
};	// Http Request
