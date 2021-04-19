#pragma once
#include <string>
#include <deque>
#include <set>

/*##############################################################################
ConfigSet
##############################################################################*/
struct ConfigSet
{
	std::string							root;
	std::set<std::string>				error_page;
	std::set<std::string>				index;
	u_int16_t							method;
	u_int32_t							head_length;
	u_int64_t							body_length;
	bool								autoindex;
	u_int64_t							timeout;
};
