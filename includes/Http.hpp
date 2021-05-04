#pragma once
struct Http;
#include <unistd.h>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include "Method.hpp"
#include "Utils.hpp"
#include "Stream.hpp"

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
	Members
	--------------------------------------------------------------------------*/
	string							headers[NUM_HEADERS];
	vector<uint8_t>					body;
	string							protocol;
	ssize_t							content_length;
	Stream							stream;

	/*--------------------------------------------------------------------------
	Static Members
	--------------------------------------------------------------------------*/
	static map<string, uint16_t>	map_method;
	static map<string, uint16_t>	map_header;

	/*--------------------------------------------------------------------------
	Methods
	--------------------------------------------------------------------------*/
	private:
	/*constructor*/					Http();
	public:
	/*constructor*/					Http(int sock_fd);

	/*--------------------------------------------------------------------------
	Static Methods
	--------------------------------------------------------------------------*/
	static void						init_map_methods();
	static void						init_map_headers();
	/*--------------------------------------------------------------------------
	Exceptions
	--------------------------------------------------------------------------*/
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};	// Http
