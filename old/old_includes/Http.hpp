#pragma once
#include <unistd.h>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include "Utils.hpp"
#include "Stream.hpp"
#include "Enums.hpp"

/*##############################################################################
Http
##############################################################################*/

using std::string;
using std::map;
using std::vector;

struct Http
{
	/*--------------------------------------------------------------------------
	typedef
	--------------------------------------------------------------------------*/

	/*--------------------------------------------------------------------------
	Members
	--------------------------------------------------------------------------*/
	map<string, string>				headers;
	string							protocol;
	ssize_t							content_length;
	Stream							stream;

	/*--------------------------------------------------------------------------
	Static Members
	--------------------------------------------------------------------------*/
	static map<string, uint32_t>	map_method;

	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
	private:
	/*constructor*/					Http();
	
	public:
	/*constructor*/					Http(int sock_fd);
	void							set_header(const string& line);
	virtual	void					clear() = 0;

	/*--------------------------------------------------------------------------
	Static Methods
	--------------------------------------------------------------------------*/
	static void						init_map_methods();

	/*--------------------------------------------------------------------------
	Exceptions
	--------------------------------------------------------------------------*/
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};	// Http
