#pragma once
#include <string>
#include <deque>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
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
	std::string		auth;
	static Method	methodSet[9];
	
	Location();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
	Location(const ConfigSet&);
	Location(std::deque<std::string>&);
	~Location();
	
	class AuthFailed: public std::exception{
		virtual const char* what() const throw();
	};
};

std::ostream&	operator<<(std::ostream&, Location&);
