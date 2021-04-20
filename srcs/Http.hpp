#pragma once
struct Http;
#include <unistd.h>
#include <string>
#include <map>
#include <deque>
#include "Method.hpp"
#include "Url.hpp"
#include "Path.hpp"
#include "Utils.hpp"
// #include "Client.hpp"

/*##############################################################################
Http
##############################################################################*/
struct Http
{
	/*--------------------------------------------------------------------------
	typedef
	--------------------------------------------------------------------------*/
	typedef std::map<std::string, std::string>	header_map;
	typedef header_map::iterator				header_iterator;

	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	std::map<std::string, std::string>			headers;
	std::deque<uint8_t>							body;
	std::string									protocol;
	static std::map<std::string, u_int16_t>		mapMethod;
	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	public:
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};	// Http
