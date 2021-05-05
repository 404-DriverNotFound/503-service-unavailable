#include "Method.hpp"

struct MethodPost: public Method
{
	/*constructor*/		MethodPost(HttpReq& req, HttpRes& res, Server& server, Location& location);

	void				load_response_header();
};
