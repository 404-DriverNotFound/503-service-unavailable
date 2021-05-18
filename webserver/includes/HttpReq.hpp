#pragma once
#include "Http.hpp"
#include "Path.hpp"

using std::string;
/*##############################################################################
	
	Http Request

##############################################################################*/
class HttpReq : public Http
{
	/*==========================================================================
		Member
	==========================================================================*/
	private:
		Path			_path;
		string			_method;

	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		HttpReq(const HttpReq& ref);
		HttpReq&	operator=(const HttpReq& ref);
		HttpReq();
	public:
		HttpReq(int fd);
		~HttpReq();

	/*==========================================================================
		Method
	==========================================================================*/
		bool			set_chunked_length();
		bool			stream_to_body();
		bool			read_crlf();
		void			clear();
	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void			set_start_line();
		void			set_stream_fd(int fd);
		void			set_stream_file_fd();
		bool			set_index_page(const set<string>& pages);


	/*==========================================================================
		Getter
	==========================================================================*/
		const string&	get_method() const;
		Path&			get_path();
};
