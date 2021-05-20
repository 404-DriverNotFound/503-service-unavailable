#include "../includes/HttpRes.hpp"
#include "../includes/Webserver.hpp"

/*##############################################################################
	
	Http Response

##############################################################################*/

/*==============================================================================
	Constructor & Destructor
==============================================================================*/
HttpRes::HttpRes(int fd)
: Http(fd),
  _len_msg(0),
  _len_send(0),
  _status_code(200)
{
	set_stream_fd(fd);
}
//------------------------------------------------------------------------------
HttpRes::~HttpRes()
{
}
/*==============================================================================
	Setter
==============================================================================*/
void		HttpRes::set_status_code(int code)
{
	if (_status_code < code)
		_status_code = code;
}
//------------------------------------------------------------------------------

void		HttpRes::set_autoindex_page(const string& path)
{
	ft::make_dir_list(path, _file->get_fd());
}
//------------------------------------------------------------------------------
void			HttpRes::set_stream_fd(int fd)
{
	_stream.set_fd_out(fd);
}
//------------------------------------------------------------------------------
void			HttpRes::set_stream_file_fd()
{
	_stream.set_fd_in(_file->get_fd());
}
//------------------------------------------------------------------------------
void			HttpRes::set_file(File::flag f)
{
	_file = new File(f);
	_stream.set_fd_in(_file->get_fd());
}
//------------------------------------------------------------------------------
void			HttpRes::set_file(const string& path, File::flag f)
{
	_file = new File(path, f);
	_stream.set_fd_in(_file->get_fd());
}
//------------------------------------------------------------------------------
void			HttpRes::set_len_msg(size_t size)
{
	_len_msg = size;
}
//------------------------------------------------------------------------------
void			HttpRes::set_len_send(size_t size)
{
	_len_send = size;
}

/*==============================================================================
	Getter
==============================================================================*/

/*==============================================================================
	Method
==============================================================================*/

//------------------------------------------------------------------------------
bool			HttpRes::load_body()
{
	if (!_len_msg)
		return false;
	_len_send += _stream.pass();

	cout << "send msg: " << _len_send << " / " << _len_msg << endl;
	
	if (_len_send >= _len_msg)
		return true;
	return false;
}
//------------------------------------------------------------------------------

string		HttpRes::get_start_line()
{
	string	startline;
	startline = string("HTTP/1.1 ");
	startline += ft::itoa(_status_code);
	startline += " ";
	startline += Webserver::get_status_code(_status_code);
	startline += "\r\n";
	return startline;
}

//------------------------------------------------------------------------------

string		HttpRes::get_allow(set<string>& method_set)
{
	string	line("Allow: ");
	// TODO location set으로 반복처리

	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_content_language(const string& accept_language)
{
	string	line("Content-Language: ");
	//TODO: 
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_content_length()
{
	string	line("Content-length: ");
	line += ft::itoa(_file->size());
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_last_modified(const char* path)
{
	string	line("Last-Modified: ");
	line += ft::get_last_modified(path);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_location(const string& redirected)
{
	string	line("Location: ");
	line += redirected;
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_retry_after(Time time)
{
	string	line("Retry-After: ");
	line += ft::itoa(time.get_time_sec());
	line += "\r\n";
	return line;
}
//------------------------------------------------------------------------------

string		HttpRes::get_retry_after(ft::Date time)
{
	string	line("Retry-After: ");
	line += ft::date_to_str(time);
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_server()
{
	string	line("Server: ");
	line += std::string("Webserver42/1.0.0 (Unix)");
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_transfer_encoding()
{
	string	line("Transfer-Encoding: chunked");
	//TODO: transfer encoding
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

string		HttpRes::get_www_authenticate(const string& realm)
{
	string	line("WWW-Authenticate: ");
	line += string("Basic realm=");
	line += realm;
	line += ", charset=\"UTF-8\"";
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

void		HttpRes::clear()
{
	Http::clear();
	_status_code = 200;
	_len_msg = 0;
	_len_send = 0;
}

int			HttpRes::get_status_code()
{
	return	_status_code;
}

// void		HttpRes::clear()
// {
// 	#ifdef DBG
// 	cout << "Res clear\n";
// 	#endif
	
// 	headers.clear();
// 	protocol.clear();
// 	content_length = 0;
	
// 	status_code = 0;
// 	msg_length = 0;
// 	send_length = 0;
// 	stream.clear();
// }

//------------------------------------------------------------------------------
