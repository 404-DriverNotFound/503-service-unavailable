#include "../includes/MethodPost.hpp"

// #define DBG

/*##############################################################################
POST
##############################################################################*/

/*constructor*/		MethodPost::MethodPost(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	req.set_extension();
	cout << "MethodPost : " << server.temp_file_dir << endl;
	if (Cgi::cgi_bin.find(req.extension) != Cgi::cgi_bin.end())
	{
		open_option = OPEN_POST_CGI;
		open_file(OPEN_POST_CGI);
		cgi = new Cgi(req.path_translated, req.extension, fd_in, fd_out);
		make_meta_variable();
		if (req.headers["TRANSFER_ENCODING"] == "chunked")
		{
			status_chunked = CHUNKED_SIZE;
			status = METHOD_RECV_CHUNKED_BODY;
		}
		else
		{
			req.stream.pass_remain = ft::atoi(req.headers["CONTNET_LENGTH"]);
			status = METHOD_RECV_BODY;
		}
		#ifdef DBG
		cout << __func__ << endl;
		#endif
	}
	else if (ft::is_dir(req.path_translated.c_str()))
	{
		open_option = OPEN_POST_DIR;
		open_file(OPEN_POST_DIR);
		if (req.headers["TRANSFER_ENCODING"] == "chunked")
		{
			status_chunked = CHUNKED_SIZE;
			status = METHOD_RECV_CHUNKED_BODY;
		}
		else
		{
			req.stream.pass_remain = ft::atoi(req.headers["CONTNET_LENGTH"]);
			status = METHOD_RECV_BODY;
		}
	}
	else
	{
		open_option = OPEN_POST;
		open_file(OPEN_POST);
		if (req.headers["TRANSFER_ENCODING"] == "chunked")
		{
			status_chunked = CHUNKED_SIZE;
			status = METHOD_RECV_CHUNKED_BODY;
		}
		else
		{
			req.stream.pass_remain = ft::atoi(req.headers["CONTNET_LENGTH"]);
			status = METHOD_RECV_BODY;
		}
	}
}

void				MethodPost::load_response_header()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	res.status_code = 201;
	res.stream << res.get_startline();
	res.stream << res.get_server();
	switch(open_option)
	{
		case OPEN_POST_CGI:
			#ifdef DBG
			cout << "open_option: " << open_option << endl;
			#endif
			
			res.stream << res.get_content_length(res.content_length);
			res.stream << "\r\n";
			break;
		case OPEN_POST:
			#ifdef DBG
			cout << "open_option: " << open_option << endl;
			#endif
			
			res.stream << res.get_content_length(0);
			res.stream << "\r\n";
			res.msg_length += res.stream.size();
			res.msg_length += res.content_length;
			break;
		default:
			#ifdef DBG
			cout << "open_option: " << open_option << endl;
			#endif
			
			res.stream << res.get_content_length(res.content_length);
			res.stream << "\r\n";
			res.msg_length += res.stream.size();
			res.msg_length += res.content_length;
			break;
	}
	// if (OPEN_POST_CGI)
	// {
	// 	// res.content_length += ft::file_size(name_out.c_str());
	// 	res.stream << res.get_content_length(res.content_length);
	// }
	// else if (OPEN_POST)
	// {
	// 	res.stream << res.get_content_length(0);
	// 	res.msg_length += res.stream.size();
	// 	res.msg_length += res.content_length;
	// }
	// else
	// {

	// }

	// STOP
}