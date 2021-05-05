#pragma once
struct HttpRes;
#include "Http.hpp"
#include "Utils.hpp"
#include "Time.hpp"

/*##############################################################################
Http Response
##############################################################################*/

using std::string;

struct HttpRes : public Http
{
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	static std::map<int, std::string>	status_code_map;
	int									status_code;
	size_t								msg_length;
	size_t								send_length;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	/*constructor*/	HttpRes(int sock_fd);

	string			get_startline();

	string			get_allow(uint16_t method);
	string			get_content_language(const string& accept_language = string());
	string			get_content_length(size_t size);
	string			get_last_modified(const char* path);
	string			get_location(const string& redirected);
	string			get_retry_after(Time time);
	string			get_retry_after(ft::Date time);
	string			get_server();
	string			get_transfer_encoding();
	string			get_www_authenticate(const string& realm);
	static void		init_status_code();
	void			clear();
};	// Http Response
