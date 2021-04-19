#pragma once
struct Http;
struct HttpReq;
struct HttpRes;
#include <unistd.h>
#include <string>
#include <map>
#include <deque>
#include "Method.hpp"
#include "Url.hpp"
#include "Path.hpp"
#include "Utils.hpp"
#include "Client.hpp"

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
	std::map<std::string, std::string>	headers;
	std::deque<uint8_t>					body;
	std::string							protocol;

	/*--------------------------------------------------------------------------
	Exception
	--------------------------------------------------------------------------*/
	public:
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};	// Http

/*##############################################################################
Http Request
##############################################################################*/
struct HttpReq : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	e_method		method;
	std::string		path;
	
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void			set_start_line(const std::string& line);
	void			set_headers(const std::string& line);

	/*--------------------------------------------------------------------------
	Private Method
	--------------------------------------------------------------------------*/
	private:
	void			set_method(const std::string& token);
};	// Http Request

/*##############################################################################
Http Response
##############################################################################*/
struct HttpRes : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	int									status_code;
	static std::map<int, std::string>	status_code_map;
	std::deque<uint8_t>					body;
	Client*								cl;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	void			set_startline();
	void			set_last_modified();
	void			set_location();
	void			set_retry_after();
	void			set_server();
	void			set_transfer_encoding();
	void			set_www_authenticate();
};	// Http Response