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
		void			set_root_replace(const string& root, const set<string>& ex);
		void			set_root_front(const string& root, const set<string>& ex);
		void			set_file(File::flag flag = File::o_create);
		void			set_file(const string& path, File::flag f = File::o_read);

	/*==========================================================================
		Getter
	==========================================================================*/
		const string&	get_method() const;
		Path&			get_path();
};
