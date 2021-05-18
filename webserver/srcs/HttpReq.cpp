#include "../includes/HttpReq.hpp"
/*##############################################################################
	
	Http Request

##############################################################################*/

/*==============================================================================
	Constructor & Destructor
==============================================================================*/
HttpReq::HttpReq(int fd)
: Http(fd)
{
	set_stream_fd(fd);
}
//------------------------------------------------------------------------------
HttpReq::~HttpReq()
{
}

/*==============================================================================
	Method
==============================================================================*/

bool	HttpReq::set_chunked_length()
{
	if (_stream.get_line(_line))
	{
		_stream.set_pass_remain(ft::atoi_hex(_line));
		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------

bool	HttpReq::stream_to_body()
{
	_stream.pass(_stream.get_pass_remain());
	if (_stream.get_pass_remain() == 0)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------

bool	HttpReq::read_crlf()
{
	if (_stream.get_line(_line))
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------
void	HttpReq::clear()
{
	Http::clear();
	_path.clear();
	_method.clear();
}

/*==============================================================================
	Setter
==============================================================================*/

void	HttpReq::set_start_line()
{
	string::const_iterator			it_line = _line.begin();
	string							token;

	// method
	if (!ft::get_chr_token(_line, it_line, token, ' ') || token.empty())
		throw 400;
	ft::uppercase(token);
	_method = token;
	
	//path
	if (!ft::get_chr_token(_line, it_line, token, ' ') || token.empty())
		throw 400;
	_path.set_path(token);

	// scheme
	ft::get_chr_token(_line, it_line, _scheme, ' ');
	if (_scheme.empty())
		throw 400;
}

//------------------------------------------------------------------------------

void			HttpReq::set_stream_fd(int fd)
{
	_stream.set_fd_in(fd);
}
//------------------------------------------------------------------------------
void			HttpReq::set_stream_file_fd()
{
	_stream.set_fd_out(_file->get_fd());
}
//------------------------------------------------------------------------------
bool			HttpReq::set_index_page(const set<string>& pages)
{
	return _path.set_index_page(pages);
}

/*==============================================================================
	Getter
==============================================================================*/

Path&	HttpReq::get_path()
{
	return	_path;
}
//------------------------------------------------------------------------------
const string&	HttpReq::get_method() const
{
	return _method;
}