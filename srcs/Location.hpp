#pragma once
#include <string>
#include <deque>
#include <vector>
#include "DataClass.hpp"
#include "Method.hpp"

/*##############################################################################
Location
##############################################################################*/
struct Location : public ConfigSet
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	std::string		location;
	std::string		cgi;
	static Method	methodSet[9];
	
	Location();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
	Location(std::deque<std::string>&);
	~Location();
};

std::ostream&	operator<<(std::ostream&, Location&);
