#include "../includes/HttpReq.hpp"

HttpReq::HttpReq()
{
}

HttpReq::HttpReq(const HttpReq& ref)
{
}

HttpReq::~HttpReq()
{
}

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

bool	HttpReq::get_next_line()
{
	return _stream.get_line(_line);
}

Path&	HttpReq::get_path()
{
	return	_path;
}

const string&	HttpReq::get_method() const
{
	return _method;
}