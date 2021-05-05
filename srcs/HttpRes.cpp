#include "../includes/HttpRes.hpp"

/*##############################################################################
Http Response
##############################################################################*/

std::map<int, std::string>	HttpRes::status_code_map;

//------------------------------------------------------------------------------

/*constructor*/	HttpRes::HttpRes(int sock_fd)
: Http(sock_fd)
{}

//------------------------------------------------------------------------------

string		HttpRes::get_startline()
{
	string	startline;
	startline = string("HTTP/1.1 ");
	startline += ft::itoa(status_code);
	startline += " ";
	startline += status_code_map[status_code];
	startline += "\r\n";
	return startline;
}

//------------------------------------------------------------------------------

string		HttpRes::get_allow(uint16_t method)
{
	string	line("Allow: ");
	line += Method::get_allow(method);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_content_language(const string& accept_language)
{
	string	line("Content-Language: ");
	//TODO: 
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_content_length(size_t size)
{
	string	line("Content-length: ");
	line += ft::itoa(size);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_last_modified(const char* path)
{
	string	line("Last-Modified: ");
	line += ft::get_last_modified(path);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_location(const string& redirected)
{
	string	line("Location: ");
	line += redirected;
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_retry_after(Time time)
{
	string	line("Retry-After: ");
	line += ft::itoa(time.get_time_sec());
	line += "\r\n";
	return line;
}
//------------------------------------------------------------------------------

string		HttpRes::get_retry_after(ft::Date time)
{
	string	line("Retry-After: ");
	line += ft::date_to_str(time);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_server()
{
	string	line("Server: ");
	line += std::string("Webserver42/1.0.0 (Unix)");
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_transfer_encoding()
{
	string	line("Transfer-Encoding: chunked");
	//TODO: transfer encoding
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_www_authenticate(const string& realm)
{
	string	line("WWW-Authenticate: ");
	line += string("Basic realm=");
	line += realm;
	line += ", charset=\"UTF-8\"";
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

void		HttpRes::init_status_code()
{
	status_code_map[100] = "Continue";
	status_code_map[101] = "Switching Protocols";
	status_code_map[103] = "Early Hints";
	status_code_map[200] = "OK";
	status_code_map[201] = "Created";
	status_code_map[202] = "Accepted";
	status_code_map[203] = "Non-Authoritative Information";
	status_code_map[204] = "No Content";
	status_code_map[205] = "Reset Content";
	status_code_map[206] = "Partial Content";
	status_code_map[300] = "Multiple Choices";
	status_code_map[301] = "Moved Permanently";
	status_code_map[302] = "Found";
	status_code_map[303] = "See Other";
	status_code_map[304] = "Not Modified";
	status_code_map[307] = "Temporary Redirect";
	status_code_map[308] = "Permanent Redirect";
	status_code_map[400] = "Bad Request";
	status_code_map[401] = "Unauthorized";
	status_code_map[402] = "Payment Required";
	status_code_map[403] = "Forbidden";
	status_code_map[404] = "Not Found";
	status_code_map[405] = "Method Not Allowed";
	status_code_map[406] = "Not Acceptable";
	status_code_map[407] = "Proxy Authentication Required";
	status_code_map[408] = "Request Timeout";
	status_code_map[409] = "Conflict";
	status_code_map[410] = "Gone";
	status_code_map[411] = "Length Required";
	status_code_map[412] = "Precondition Failed";
	status_code_map[413] = "Payload Too Large";
	status_code_map[414] = "URI Too Long";
	status_code_map[415] = "Unsupported Media Type";
	status_code_map[416] = "Range Not Satisfiable";
	status_code_map[417] = "Expectation Failed";
	status_code_map[418] = "I'm a teapot";
	status_code_map[422] = "Unprocessable Entity";
	status_code_map[425] = "Too Early";
	status_code_map[426] = "Upgrade Required";
	status_code_map[428] = "Precondition Required";
	status_code_map[429] = "Too Many Requests";
	status_code_map[431] = "Request Header Fields Too Large";
	status_code_map[451] = "Unavailable For Legal Reasons";
	status_code_map[500] = "Internal Server Error";
	status_code_map[501] = "Not Implemented";
	status_code_map[502] = "Bad Gateway";
	status_code_map[503] = "Service Unavailable";
	status_code_map[504] = "Gateway Timeout";
	status_code_map[505] = "HTTP Version Not Supported";
	status_code_map[506] = "Variant Also Negotiates";
	status_code_map[507] = "Insufficient Storage";
	status_code_map[508] = "Loop Detected";
	status_code_map[510] = "Not Extended";
	status_code_map[511] = "Network Authentication Required";
}