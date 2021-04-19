#pragma once
#include <string>
#include <deque>
#include <set>
#include "Utils.hpp"

/*##############################################################################
ConfigSet
##############################################################################*/
struct ConfigSet
{
	std::string							root;
	std::set<std::string>				error_page;
	std::set<std::string>				index;
	uint16_t							method;
	uint32_t							head_length;
	uint64_t							body_length;
	bool								autoindex;
	uint64_t							timeout;
};
