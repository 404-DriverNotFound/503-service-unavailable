#pragma once
#include <string>
#include <deque>
#include <set>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "ConfigSet.hpp"
#include "Method.hpp"

/*##############################################################################
Location
##############################################################################*/

using std::string;
using std::list;
using std::set;

struct Location : public ConfigSet
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	set<string>	cgi_extension;
	string		auth_type;
	string		auth;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
	Location();
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
