#pragma once
#include <string>
#include <map>
#include "Stream.hpp"

using std::string;
using std::map;

class Http
{
	private:
		map<string, string>		_headers;
		string					_protocol;
		ssize_t					_content_length;
	public:
		Http();
		~Http();

		void	set_start_line(string& line);
		void	set_header(string& line);
		string	get_start_line(int code);
		/*=======================
		getter
		=======================*/
		map<string, string>&	get_headers();
};
