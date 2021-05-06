#include "../includes/MethodPut.hpp"

/*##############################################################################
PUT
##############################################################################*/

/*constructor*/		MethodPut::MethodPut(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	cout << __func__ << endl;
	open_file(OPEN_PUT);
	if (req.headers["TRANSFER_ENCODING"] == "chunked")
	{
		status = METHOD_RECV_CHUNKED_BODY;
		status_chunked = CHUNKED_SIZE;
	}
	else
	{
		req.stream.pass_remain = ft::atoi(req.headers["CONTNET_LENGTH"]);
		status = METHOD_RECV_BODY;
	}
}

void	MethodPut::load_response_header() 
{
	cout << "MethodPut::" << __func__ << endl;
	res.status_code = 201;
	res.stream << res.get_startline();
	res.stream << res.get_server();
	res.stream << res.get_content_length(6);
	res.stream << "\r\n";
	res.stream << "hello\n";
	res.msg_length = res.stream.size();
}
