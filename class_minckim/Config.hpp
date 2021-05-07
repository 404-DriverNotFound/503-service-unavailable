#pragma once
#include "Path.hpp"
#include "Method.hpp"
#include <string>


struct Config
{
	public:
	std::string			name;
	Path				root;				// def = /
	uint32_t			port;				// def = 80;
	std::vector<Path>	index_page;			// def = index.html
	Path				error_page;			// def = error.html
	uint64_t			head_length_max;	// def = 8k
	uint64_t			body_length_max;	// def = 1M
	bool				autoindex;			// def = off
	uint32_t			timeout;			// def = 5s
	Path				auth;

};