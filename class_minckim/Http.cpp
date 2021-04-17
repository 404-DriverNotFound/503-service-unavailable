#include "Http.hpp"

void		HttpReq::set_start_line(const std::string& line)
{
	std::string::const_iterator			it_line = line.begin();
	std::string							token;

	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw HttpFormException();
	set_method(token);
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw HttpFormException();
	token.swap(path);
	ft::get_chr_token(line, it_line, token, ' ');
	if (token.empty())
		throw HttpFormException();
	if (!token.empty() && token.back() == '\r')
		token.pop_back();
	token.swap(protocol);
}

void	HttpReq::set_headers(const std::string& line)
{
	std::string::const_iterator			it_line = line.begin();
	std::string							key;
	std::string							val;

	ft::get_set_token(line, it_line, key, ": ");
	while (ft::strchr(": ", *it_line))
		++it_line;
	headers[key].assign(it_line, line.end());
}

void	HttpReq::set_method(const std::string& token)
{
	if (token == "GET")
		method = GET;
	else if (token == "HEAD")
		method = HEAD;
	else if (token == "PUT")
		method = PUT;
	else if (token == "POST")
		method = POST;
	else if (token == "TRACE")
		method = TRACE;
	else if (token == "DELETE")
		method = DELETE;
	else if (token == "OPTION")
		method = OPTION;
	else
		throw HttpFormException();
}

const char*		Http::HttpFormException::what() const throw()
{
	return "HttpFormException";
}

//startline test
/*
int		main()
{
	try
	{
		std::string		str("GET /paaath/p HTTP/1.1");
		std::cout << str << "[normal test]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("POST /paaath/p HTTP/1.1");
		std::cout << str << "[normal test]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET /paaath/p      HTTP/1.1");
		std::cout << str << "[many blanks]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET /paaath/p HTTP/1.1     ");
		std::cout << str << "[blanks followed by protocol]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("get /paaath/p HTTP/1.1     ");
		std::cout << str << "[lowercase test]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("NO /paaath/p HTTP/1.1     ");
		std::cout << str << "[wrong method]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET /paaath/p");
		std::cout << str << "[no protocol]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET /paaath/p   ");
		std::cout << str << "[no protocol and blank followed by path]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET");
		std::cout << str << "[only method]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET   ");
		std::cout << str << "[only method and blanks followed by method]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("SET   ");
		std::cout << str << "[wrong method and blanks followed by method]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("  ");
		std::cout << str << "[blanks]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("");
		std::cout << str << "[empty]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << std::endl;

}
*/

/*
//header test
int			main()
{
	std::string					str("hel:    llooo: oo");
	HttpReq			r;

	r.set_headers(str);
	std::cout << r.headers.begin()->first << std::endl;
	std::cout << r.headers.begin()->second << std::endl;
}

*/