#include "../includes/HttpReq.hpp"

/*##############################################################################
Http Request
##############################################################################*/

map<int, string>	status_code_map;

//------------------------------------------------------------------------------

void		HttpReq::set_start_line(string& line)
{
	string::const_iterator			it_line = line.begin();
	string							token;

	if (!line.empty() && *--line.end() == '\r')
		line.erase(--line.end());
	// method
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw 400;
	ft::uppercase(token);
	set_method(token);
	
	//path
	if (!ft::get_chr_token(line, it_line, token, ' ') || token.empty())
		throw 400;
	set_path(token);

	// scheme
	ft::get_chr_token(line, it_line, token, ' ');
	if (token.empty())
		throw 400;
	if (!token.empty() && *--token.end() == '\r')
		token.erase(--token.end());
	token.swap(protocol);
}

//------------------------------------------------------------------------------

void		HttpReq::set_path(string& token)
{
	string::const_iterator			it_token = token.begin();

	if (ft::strncmp("http://", token.c_str(), 7) == 0)
	{
		it_token += 7;
		while (*it_token && *it_token != '/')
			++it_token;
	}
	//	path
	string		tmp;
	ft::get_chr_token(token, it_token, tmp, '?');
	ft::get_set_token(tmp, path, "/");
	//	query
	query.assign(const_cast<char*>(it_token.base()), const_cast<char*>(token.end().base()));
}

//------------------------------------------------------------------------------

void		HttpReq::set_method(const string& token)
{
	map<string, uint16_t>::iterator it = Http::map_method.find(token);
	if (it == Http::map_method.end())
		throw 400;
	method = it->second;
}

//------------------------------------------------------------------------------

void		HttpReq::set_header(string& line)
{
	string::const_iterator			it_line = line.begin();
	string							key;
	string							val;

	if (!line.empty() && *--line.end() == '\r')
		line.erase(--line.end());
	ft::get_set_token(line, it_line, key, ": ");
	ft::uppercase(key);
	while (ft::strchr(": ", *it_line))
		++it_line;
	map<string, uint16_t>::iterator	it_header = map_header.find(key);
	if (it_header == map_header.end())
		throw 400;
	headers[it_header->second] = it_line.base();
}

//------------------------------------------------------------------------------

void		HttpReq::get_location_name(string& location_name)
{
	if (path.size() == 1)
		location_name = "/";
	else
		location_name = path.front();
}


//startline test
/* 
int		main()
{
	Http::init_map_headers();
	Http::init_map_methods();
	try
	{
		std::string		str("GET /paaath/p HTTP/1.1");
		std::cout << str << "[normal test]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
	}
	std::cout << std::endl;


	try
	{
		std::string		str("GET http://naver.com:80/index.html?abc=def HTTP/1.1");
		std::cout << str << "[full path]" << std::endl;

		HttpReq			a;
		a.set_start_line(str);
		std::cout << a.protocol << std::endl;
		std::cout << a.method << std::endl;
		std::cout << a.path << std::endl;
		std::cout << a.query << std::endl;
	}
	catch(int i)
	{
		std::cerr << i << '\n';
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
