#pragma once
#include "Http.hpp"

class HttpRes : public Http
{
	private:
		int		_status_code;
		string	_scheme;
	public:
		HttpRes();
		HttpRes(const HttpRes& ref);
		~HttpRes();
		HttpRes		operator=(const HttpRes& ref);

		void		get_start_line(string& line);
		/*=======================
		setter
		=======================*/
		void		set_status_code(int code);
};
