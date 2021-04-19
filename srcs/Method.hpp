#pragma once

enum e_method
{
	CONNECT = 0,
	DELETE,
	GET,
	HEAD,
	OPTIONS,
	PATCH,
	POST,
	PUT,
	TRACE,
};

struct	Method {
	std::string	str;
	u_int16_t	flag;
};
