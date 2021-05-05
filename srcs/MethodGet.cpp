#include "../includes/MethodGet.hpp"

/*##############################################################################
POST
##############################################################################*/

/*constructor*/		MethodGet::MethodGet(HttpReq& req, HttpRes& res, Server& server, Location& location)
: Method(req, res, server, location)
{
	open_file(OPEN_GET);
	status = METHOD_LOAD_HEADER;
}