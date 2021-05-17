#pragma once
#include <string>
#include <map>
#include "Stream.hpp"
#include "File.hpp"

using std::string;
using std::map;

class Http
{
	protected:
		map<string, string>		_headers;
		string					_protocol;
		string					_line;
		Stream					_stream;
		File*					_file;
		ssize_t					_content_length;
	public:
		Http();
		~Http();

		void					set_start_line(string& line);
		void					set_header(string& line);
		string					get_start_line(int code);
		void					set_file(File::flag flag = File::flag::o_create);
		void					set_file(const string& path, File::flag f = File::flag::o_read);
		/*=======================
		getter
		=======================*/
		map<string, string>&	get_headers();
		Stream&					get_stream();
		string&					get_string();
};
