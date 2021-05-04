#pragma once
struct Http;
#include <unistd.h>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include "Method.hpp"
// #include "Url.hpp"
// #include "Path.hpp"
#include "Utils.hpp"
// #include "Client.hpp"

/*##############################################################################
Http
##############################################################################*/

enum e_header
{
	ACCEPT_CHARSET,
	ACCEPT_LANGUAGE,
	AUTHORIZATION,
	HOST,
	REFERER,
	USER_AGENT,
	LAST_MODIFIED,
	LOCATION,
	RETRY_AFTER,
	SERVER,
	TRANSFER_ENCODING,
	WWW_AUTHENTICATE,
	ALLOW,
	CONTNET_LENGTH,
	CONTENT_LOCATION,
	CONTENT_TYPE,
	DATE,
	STATUS,
	NUM_HEADERS,
};

using std::string;
using std::map;
using std::vector;

struct Http
{
	/*--------------------------------------------------------------------------
	typedef
	--------------------------------------------------------------------------*/

	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	string							headers[NUM_HEADERS];
	vector<uint8_t>					body;
	string							protocol;
	static map<string, uint16_t>	map_method;
	static map<string, uint16_t>	map_header;
	ssize_t							content_length;

									Http();
	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	public:
	static void						init_map_methods();
	static void						init_map_headers();
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};	// Http
