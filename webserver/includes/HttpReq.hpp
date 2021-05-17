#pragma once
#include "Http.hpp"
#include "Path.hpp"

using std::string;

class HttpReq : public Http
{
	private:
		Path	_path;
		string	_method;
		string	_scheme;
	public:
		HttpReq();
		HttpReq(const HttpReq& ref);
		~HttpReq();
		HttpReq		operator=(const HttpReq& ref);

		void			set_start_line();
		bool			get_next_line();
		const string&	get_method() const;
		Path&			get_path();
};
