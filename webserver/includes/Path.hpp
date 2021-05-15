#pragma once
#include <string>
#include <list>
#include <iomanip>
#include <iostream>

using std::list;
using std::string;


class Path
{
	enum flag_path
	{
		f_is_cgi,
		f_is_file,
		f_is_dir,
		f_is_exist,
	};

	private:
		list<string>	_segments;
		int				_flag;

	public:
		bool			is_file();
		bool			is_exist();
		bool			is_dir();

};