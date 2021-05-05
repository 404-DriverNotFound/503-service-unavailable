#include "../includes/MethodPost.hpp"

/*##############################################################################
POST
##############################################################################*/

/*constructor*/		MethodPost::MethodPost(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	if (Cgi::cgi_bin.find(req.extension) != Cgi::cgi_bin.end())
	{
		open_file(OPEN_POST_CGI);
		cgi = new Cgi(req.path_translated, req.extension, fd_in, fd_out, make_meta_variable());
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
	else
	{
		open_file(OPEN_POST);
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
}

