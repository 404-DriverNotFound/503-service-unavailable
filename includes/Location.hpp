#pragma once
#include <string>
#include <deque>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "ConfigSet.hpp"
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
	std::string		auth_type;
	std::string		auth;
	
	Location();
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
	Location(const Location&);
	Location(std::deque<std::string>&);
	~Location();
	
	class AuthFailed: public std::exception{
		virtual const char* what() const throw();
	};
	class InvalidMethod: public std::exception{
		virtual const char* what() const throw();
	};
};

std::ostream&	operator<<(std::ostream&, Location&);
