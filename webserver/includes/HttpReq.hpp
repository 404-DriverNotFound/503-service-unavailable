#pragma once

#include "ClassModule.hpp"
using std::string;

class HttpReq : public Http
{
	private:
		Path&	_path;
		string	query;
		string	scheme;
	public:
		HttpReq();
		HttpReq(const HttpReq& ref);
		~HttpReq();
		HttpReq		operator=(const HttpReq& ref);

		void		set_start_line(string& line);
};