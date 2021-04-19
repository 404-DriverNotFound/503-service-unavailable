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
	Location();
	static Method	methodSet[9];
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
	Location(std::deque<std::string>&);
	~Location();
};

std::ostream&	operator<<(std::ostream&, Location&);
