#include "Method.hpp"

class MethodGet: public Method
{
	/*constructor*/	MethodGet(HttpReq& req, HttpRes& res, Server& server, Location& location);
};