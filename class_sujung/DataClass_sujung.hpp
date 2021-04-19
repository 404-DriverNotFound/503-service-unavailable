#pragma once
#include <string>
#include <vector>
#include <deque>

struct ConfigSet
{
		std::string					root;
		std::vector<std::string>	error_page;
		std::vector<std::string>	index;
		std::vector<std::string>	method;
		u_int32_t					head_length;
		u_int64_t					body_length;
		bool						autoindex;
		u_int64_t					timeout;
};

typedef struct	s_charset
{
	std::string		str;
	
}				t_charset;
