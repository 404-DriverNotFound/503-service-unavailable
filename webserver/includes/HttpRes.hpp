#pragma once
#include "Http.hpp"
#include "Time.hpp"
#include "ConfigLocation.hpp"

/*##############################################################################
	
	Http Response

##############################################################################*/
class HttpRes : public Http
{
	/*==========================================================================
		Member
	==========================================================================*/
	private:
		int		_status_code;
		ssize_t	_len_msg;
		ssize_t	_len_send;
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
		HttpRes(const HttpRes& ref);
		HttpRes&	operator=(const HttpRes& ref);
		HttpRes();
	public:
		HttpRes(int fd);
		~HttpRes();

	/*==========================================================================
		Method
	==========================================================================*/
		string		get_allow(set<string>& method_set);
		string		get_content_language(const string& accept_language = string());
		string		get_content_length();
		string		get_last_modified(const char* path);
		string		get_location(const string& redirected);
		string		get_retry_after(Time time);
		string		get_retry_after(ft::Date time);
		string		get_server();
		string		get_transfer_encoding();
		string		get_www_authenticate(const string& realm);
		string		get_start_line();

		void		clear();
		bool		load_body();

	/*==========================================================================
		Setter
	==========================================================================*/
		void		set_status_code(int code);
		void		set_autoindex_page(const ConfigLocation& location, const string& path);
		void		set_stream_fd(int fd);
		void		set_stream_file_fd();
		void		set_file(File::flag flag = File::o_create);
		void		set_file(const string& path, File::flag f = File::o_read);
		void		set_len_msg(size_t size);
		void		set_len_send(size_t size);

	/*==========================================================================
		Getter
	==========================================================================*/
		int			get_status_code();
};
