#include "Method.hpp"

class MethodPut: public Method
{
	/*constructor*/		MethodPut(HttpReq& req, HttpRes& res, Server& server, Location& location);
};