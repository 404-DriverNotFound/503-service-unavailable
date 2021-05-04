#include "../includes/MethodPost.hpp"

/*##############################################################################
POST
##############################################################################*/

/*constructor*/		MethodPost::MethodPost(HttpReq& req, HttpRes& res)
: Method(req, res)
{
	if (req.headers[TRANSFER_ENCODING] == "chunked")
	{
		status = METHOD_RECV_CHUNKED_BODY;
	}
	else
	{
		req.stream.pass_remain = ft::atoi(req.headers[CONTNET_LENGTH]);
		status = METHOD_RECV_BODY;
	}
	if (Cgi::cgi_bin.find(req.extension) != Cgi::cgi_bin.end())
	{
		open_in_file();
		open_out_file();
		cgi = new Cgi(req.path_translated, req.extension, fd_in, fd_out, make_meta_variable());
	}
}

bool	MethodPost::run()
{
	switch (status)
	{
	case METHOD_RECV_BODY:
		req.stream.pass(req.stream.pass_remain);
		if (req.stream.pass_remain == 0)
		{
			if (cgi)
			{
				status = METHOD_START_CGI;
				// goto
			}
			else
			{
				status = METHOD_LOAD_HEADER;
				// goto
			}
		}
		if (status == METHOD_RECV_BODY)
			break;

	case METHOD_RECV_CHUNKED_BODY:
		if (recv_chunked_body())
		{
			if (cgi)
			{
				status = METHOD_START_CGI;
				// goto
			}
			else
			{
				status = METHOD_LOAD_HEADER;
				// goto
			}
		}
		if (status == METHOD_RECV_CHUNKED_BODY)
			break;

	case METHOD_START_CGI:
		run_cgi();
		status = METHOD_CGI_IS_RUNNING;

	case METHOD_CGI_IS_RUNNING:


		if (cgi->check_exit())
			status = METHOD_LOAD_HEADER;
		if (status == METHOD_CGI_IS_RUNNING)
			break;


	case METHOD_LOAD_HEADER:
		set_cgi_header();
		load_response_header();
		load_cgi_tmp_remain();
		status = METHOD_LOAD_BODY;

	case METHOD_LOAD_BODY:
		load_body();
		if (status == METHOD_LOAD_BODY)
			break;

	case METHOD_DONE:
		break;

	default:
		break;
	}
}