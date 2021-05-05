#include "Method.hpp"

class MethodPost: public Method
{
	/*constructor*/		MethodPost(HttpReq& req, HttpRes& res, Server& server, Location& location);
};