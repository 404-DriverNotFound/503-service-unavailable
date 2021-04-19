#pragma once
#include "Utils.hpp"

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
	uint16_t	flag;
};
