#pragma once
#include <string>
#include <deque>
#include <set>
#include "Utils.hpp"

/*##############################################################################
ConfigSet
##############################################################################*/
class ConfigSet
{
	/*--------------------------------------------------------------------------
	Members
	--------------------------------------------------------------------------*/
	std::string							name;
	std::string							root;
	std::set<std::string>				error_page;
	std::set<std::string>				index;
	uint32_t							method;
	uint32_t							head_length;
	uint64_t							body_length;
	bool								autoindex;
	uint64_t							timeout;

	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
					ConfigSet();
					ConfigSet(const ConfigSet& x);
					~ConfigSet();
	ConfigSet&		operator=(const ConfigSet& x);

	/*--------------------------------------------------------------------------
	Exceptions
	--------------------------------------------------------------------------*/
	class InvalidConfig : public std::exception
	{
		public: virtual const char* what() const throw();
	};
};
