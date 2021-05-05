#include "../includes/MethodGet.hpp"

/*##############################################################################
GET
##############################################################################*/

/*constructor*/		MethodGet::MethodGet(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	cout << __func__ << endl;
	open_file(OPEN_GET);
	status = METHOD_LOAD_HEADER;
}

void	MethodGet::load_response_header() {
	cout << __func__ << endl;
	res.status_code = 200;
	res.stream << res.get_startline();
	res.stream << res.get_content_length(ft::file_size(req.path_translated.c_str()));
	res.stream << res.get_server();
	res.stream << "\r\n";
	load_body();
}
