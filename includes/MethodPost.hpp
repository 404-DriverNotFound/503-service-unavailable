#include "Method.hpp"

class MethodPost: public Method
{
	/*constructor*/		MethodPost(HttpReq& req, HttpRes& res);
	/*destructor*/		~MethodPost();
	bool				run();
};