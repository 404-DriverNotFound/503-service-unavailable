#pragma once
#include "Method.hpp"

struct MethodPut: public Method
{
	/*constructor*/		MethodPut(HttpReq& req, HttpRes& res, Server& server, Location& location);
	void				load_response_header(void);
};
