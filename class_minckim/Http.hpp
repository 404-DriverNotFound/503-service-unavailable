#pragma once
#include <string>
#include <map>
#include <deque>
#include "Method.hpp"
#include "Url.hpp"
#include "Path.hpp"
#include "Utils.hpp"

struct Http
{
	typedef std::map<std::string, std::string>	header_map;
	typedef header_map::iterator				header_iterator;
	std::map<std::string, std::string>	headers;
	std::deque<uint8_t>					body;
	std::string							protocol;

	//exceptions
	public:
	class HttpFormException : public std::exception{
		const char* what() const throw();	};
};

struct HttpReq : public Http
{
	e_method		method;
	std::string		path;
	
	void			set_start_line(const std::string& line);
	void			set_headers(const std::string& line);

	private:
	void			set_method(const std::string& token);
};

struct HttpRes : public Http
{
	int										status_code;
	static std::map<int, std::string>		map_code;
	std::deque<uint8_t>						body;
};
