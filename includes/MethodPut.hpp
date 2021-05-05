#pragma once
#include "Method.hpp"

struct MethodPut: public Method
{
	/*constructor*/		MethodPut(HttpReq& req, HttpRes& res, Server& server, Location& location);
};
