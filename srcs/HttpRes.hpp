#pragma once
struct HttpRes;
#include "HttpReq.hpp"
#include "Http.hpp"

/*##############################################################################
Http Response
##############################################################################*/
struct HttpRes : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	int									status_code;
	static std::map<int, std::string>	status_code_map;
	std::deque<uint8_t>					body;
	Client*								cl;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void			set_startline();
	void			set_last_modified();
	void			set_location();
	void			set_retry_after();
	void			set_server();
	void			set_transfer_encoding();
	void			set_www_authenticate();
};	// Http Response
