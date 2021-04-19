#include "ConfigWebserver.hpp"

void		ConfigWebserver::set_path_cgi_bin(char** env)
{
	Cgi::cgi_bin["php"] = ft::which("php", env);
	Cgi::cgi_bin["py"] = ft::which("python3", env);
}

void		ConfigWebserver::set_status_code()
{
	HttpRes::status_code_map[100] = "Continue";
	HttpRes::status_code_map[101] = "Switching Protocols";
	HttpRes::status_code_map[103] = "Early Hints";
	HttpRes::status_code_map[200] = "OK";
	HttpRes::status_code_map[201] = "Created";
	HttpRes::status_code_map[202] = "Accepted";
	HttpRes::status_code_map[203] = "Non-Authoritative Information";
	HttpRes::status_code_map[204] = "No Content";
	HttpRes::status_code_map[205] = "Reset Content";
	HttpRes::status_code_map[206] = "Partial Content";
	HttpRes::status_code_map[300] = "Multiple Choices";
	HttpRes::status_code_map[301] = "Moved Permanently";
	HttpRes::status_code_map[302] = "Found";
	HttpRes::status_code_map[303] = "See Other";
	HttpRes::status_code_map[304] = "Not Modified";
	HttpRes::status_code_map[307] = "Temporary Redirect";
	HttpRes::status_code_map[308] = "Permanent Redirect";
	HttpRes::status_code_map[400] = "Bad Request";
	HttpRes::status_code_map[401] = "Unauthorized";
	HttpRes::status_code_map[402] = "Payment Required";
	HttpRes::status_code_map[403] = "Forbidden";
	HttpRes::status_code_map[404] = "Not Found";
	HttpRes::status_code_map[405] = "Method Not Allowed";
	HttpRes::status_code_map[406] = "Not Acceptable";
	HttpRes::status_code_map[407] = "Proxy Authentication Required";
	HttpRes::status_code_map[408] = "Request Timeout";
	HttpRes::status_code_map[409] = "Conflict";
	HttpRes::status_code_map[410] = "Gone";
	HttpRes::status_code_map[411] = "Length Required";
	HttpRes::status_code_map[412] = "Precondition Failed";
	HttpRes::status_code_map[413] = "Payload Too Large";
	HttpRes::status_code_map[414] = "URI Too Long";
	HttpRes::status_code_map[415] = "Unsupported Media Type";
	HttpRes::status_code_map[416] = "Range Not Satisfiable";
	HttpRes::status_code_map[417] = "Expectation Failed";
	HttpRes::status_code_map[418] = "I'm a teapot";
	HttpRes::status_code_map[422] = "Unprocessable Entity";
	HttpRes::status_code_map[425] = "Too Early";
	HttpRes::status_code_map[426] = "Upgrade Required";
	HttpRes::status_code_map[428] = "Precondition Required";
	HttpRes::status_code_map[429] = "Too Many Requests";
	HttpRes::status_code_map[431] = "Request Header Fields Too Large";
	HttpRes::status_code_map[451] = "Unavailable For Legal Reasons";
	HttpRes::status_code_map[500] = "Internal Server Error";
	HttpRes::status_code_map[501] = "Not Implemented";
	HttpRes::status_code_map[502] = "Bad Gateway";
	HttpRes::status_code_map[503] = "Service Unavailable";
	HttpRes::status_code_map[504] = "Gateway Timeout";
	HttpRes::status_code_map[505] = "HTTP Version Not Supported";
	HttpRes::status_code_map[506] = "Variant Also Negotiates";
	HttpRes::status_code_map[507] = "Insufficient Storage";
	HttpRes::status_code_map[508] = "Loop Detected";
	HttpRes::status_code_map[510] = "Not Extended";
	HttpRes::status_code_map[511] = "Network Authentication Required";
}