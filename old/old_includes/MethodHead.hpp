#pragma once
#include "Method.hpp"

struct MethodHead: public Method
{
	/*constructor*/	MethodHead(HttpReq& req, HttpRes& res, Server& server, Location& location);
	void			load_response_header(void);
};
