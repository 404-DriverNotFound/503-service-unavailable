#include "Method.hpp"

class MethodGet: public Method
{
	/*constructor*/	MethodGet(HttpReq& req, HttpRes& res, Server& server, Location& location);
	void	load_response_header(void);
};
