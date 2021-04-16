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
	std::multimap<std::string, std::string>	headers;
	std::deque<uint8_t>						body;
	std::string								protocol;
	int										version_major;
	int										version_minor;
};

struct HttpReq : public Http
{
	e_method								method;
	std::string								path;

	void									set_start_line(const std::string& line);
};

struct HttpRes : public Http
{
	int										status_code;
	static std::map<int, std::string>		map_code;
};
