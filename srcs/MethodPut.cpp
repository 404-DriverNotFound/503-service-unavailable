#include "../includes/MethodPut.hpp"

/*##############################################################################
PUT
##############################################################################*/

/*constructor*/		MethodPut::MethodPut(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	open_file(OPEN_PUT);
	if (req.headers[TRANSFER_ENCODING] == "chunked")
	{
		status = METHOD_RECV_CHUNKED_BODY;
	}
	else
	{
		req.stream.pass_remain = ft::atoi(req.headers[CONTNET_LENGTH]);
		status = METHOD_RECV_BODY;
	}
}

