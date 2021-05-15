#pragma once
#include "Http.hpp"

class HttpRes : public Http
{
	private:
		Path&	_path;
		string	_query;
		string	scheme;
	public:
		HttpRes();
		HttpRes(const HttpRes& ref);
		~HttpRes();
		HttpRes		operator=(const HttpRes& ref);

		void		get_start_line(string& line);
};