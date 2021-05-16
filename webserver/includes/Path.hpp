#pragma once
#include "Tokenizer.hpp"
#include <string>
#include <list>
#include <iomanip>
#include <iostream>

using std::list;
using std::string;

/*##############################################################################
Config Global
##############################################################################*/

class Path
{
	enum flag_path
	{
		f_is_cgi,
		f_is_file,
		f_is_dir,
		f_is_exist,
	};
	/*==========================================================================
		Member
	==========================================================================*/
	public:
	// private:
		string			_path_info;
		list<string>	_segments;
		string			_extension;
		int				_flag;
		string			_path_translated;

	public:
		Path();
		Path(string& raw, string& query);
		Path(const string& ref);
		~Path();
		Path&	operator=(Path& ref);

	public:
		void			set_root();


		bool			is_cgi();
		bool			is_file();
		bool			is_exist();
		bool			is_dir();

	private:
		void			arrange_segment(list<string>& segments);
		void			set_extension(string& segment);
};