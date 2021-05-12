#pragma once
#include <string>
using std::string;

class Http
{
	private:
	public:
		Http();
		~Http();

		void	set_start_line(string& line);
		void	set_header(string& line);
		string	get_start_line(int code);
};
