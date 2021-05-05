#include "../includes/Method.hpp"

map<string, uint32_t>	Method::method_num;
map<string, uint32_t>	Method::method_flags;
string					Method::method_strings[NUM_METHOD];

//------------------------------------------------------------------------------

		Method::Method(HttpReq& req, HttpRes& res, Server& server, Location& location)
:req(req), res(res), cgi(0), server(server), location(location), fd_in(-1), fd_out(-1)
{}

//------------------------------------------------------------------------------

		Method::~Method()
{
	if (cgi)
		delete cgi;
}

//------------------------------------------------------------------------------

bool	Method::recv_body()
{
	return !req.stream.pass(req.stream.pass_remain);
}

//------------------------------------------------------------------------------

bool	Method::recv_chunked_body()
{
	while (42)
	{
		switch (status_chunked)
		{
			case CHUNKED_SIZE:
				if (req.stream.get_line(req.line))
				{
					req.stream.pass_remain = ft::atoi_hex(req.line);
					if (req.stream.pass_remain == 0)
					{
						return true;
					}
					else
					{
						status_chunked = CHUNKED_RECV;
					}
				}
				else
				{
					return false;
				}
			case CHUNKED_RECV:
				req.stream.pass(req.stream.pass_remain);
				if (req.stream.pass_remain == 0)
				{
					status_chunked = CHUNKED_NL;
				}
				else
				{
					return false;
				}
			case CHUNKED_NL:
				if (req.stream.get_line(req.line))
				{
					status_chunked = CHUNKED_SIZE;
				}
				else
				{
					return false;
				}
		}
	}
	return false;
}

//------------------------------------------------------------------------------

void	Method::run_cgi()
{
	cgi->start_cgi();
}

//------------------------------------------------------------------------------

void	Method::set_cgi_header()
{
	Stream	stream(8000, fd_out);
	string	line;

	stream.fill(location.head_length);
	while (stream.get_line(line))
	{
		res.content_length -= line.length();
		res.content_length -= 2;
		if (line.empty())
			break ;
		res.set_header(line);
	}
	load_response_header();
	res.stream.write(stream.it_buffer, stream.size());
}

//------------------------------------------------------------------------------

void	Method::load_response_header()
{
	res.status_code = 200;
	res.stream << res.get_startline();
	res.stream << res.get_content_length(ft::file_size(req.path_translated.c_str()));
	res.stream << res.get_server();
	res.stream << "\r\n";
}

//------------------------------------------------------------------------------

void	Method::load_body()
{
	if (res.stream.fill(res.stream.default_capacity) == 0)
		status = METHOD_DONE;
}

//------------------------------------------------------------------------------

bool	Method::run()
{
	switch (status)
	{
	case METHOD_RECV_BODY:
		if (recv_body())
		{
			if (cgi)
			{
				status = METHOD_START_CGI;
				goto METHOD_START_CGI;
			}
			else
			{
				status = METHOD_LOAD_HEADER;
				goto METHOD_LOAD_HEADER;
			}
		}
		if (status == METHOD_RECV_BODY)
			break;

	case METHOD_RECV_CHUNKED_BODY:
		if (recv_chunked_body())
		{
			if (cgi)
			{
				status = METHOD_START_CGI;
				goto METHOD_START_CGI;
			}
			else
			{
				status = METHOD_LOAD_HEADER;
				goto METHOD_LOAD_HEADER;
			}
		}
		if (status == METHOD_RECV_CHUNKED_BODY)
			break;

	case METHOD_START_CGI:
	METHOD_START_CGI:
		run_cgi();
		status = METHOD_CGI_IS_RUNNING;

	case METHOD_CGI_IS_RUNNING:
		if (cgi->check_exit())
		{
			status = METHOD_LOAD_HEADER_CGI;
			goto METHOD_LOAD_HEADER_CGI;
		}
		if (status == METHOD_CGI_IS_RUNNING)
			break;
		
	case METHOD_LOAD_HEADER:
	METHOD_LOAD_HEADER:
		load_response_header();
		status = METHOD_DONE;
		goto METHOD_DONE;
		

	case METHOD_LOAD_HEADER_CGI:
	METHOD_LOAD_HEADER_CGI:
		set_cgi_header();
		status = METHOD_LOAD_BODY;

	case METHOD_LOAD_BODY:
	METHOD_LOAD_BODY:
		load_body();
		if (status == METHOD_LOAD_BODY)
			break;

	case METHOD_DONE:
	METHOD_DONE:
		return true;

	default:
		break;
	}
	return false;
}

//------------------------------------------------------------------------------

void		Method::open_file_base(const string& path)
{
	if (ft::is_dir(path.c_str()))
	{
		string	path_tmp = ft::find(path, location.index);
		if (path_tmp.empty())
			throw 404;
		req.path_translated.append("/");
		req.path_translated.append(path_tmp);
	}
}

//------------------------------------------------------------------------------

void		Method::open_file_in(const string& path)
{
	open_file_base(path);
	fd_in = open(req.path_translated.c_str(), O_RDONLY);
}

//------------------------------------------------------------------------------

void		Method::open_file_out(const string& path)
{
	open_file_base(path);
	fd_out = open(req.path_translated.c_str(), O_RDONLY);
}

//------------------------------------------------------------------------------

void		Method::open_file(e_openfile option)
{
	switch (option)
	{
	case OPEN_GET:
		open_file_base(req.path_translated);
		fd_out = open(req.path_translated.c_str(), O_RDONLY);
		if (fd_out < 0)
			throw 500;
		break;

	case OPEN_PUT:
		if (ft::is_dir(req.path_translated.c_str()))
			throw 404;
		fd_in = open(req.path_translated.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (fd_in < 0)
			throw 500;
		break;

	case OPEN_POST:
		if (ft::is_dir(req.path_translated.c_str()))
			throw 404;
		fd_in = open(req.path_translated.c_str(), O_CREAT | O_APPEND | O_RDWR, 0644);
		break;

	case OPEN_POST_CGI:
		name_in = temp_name();
		name_out = temp_name();
		fd_in = open(name_in.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		fd_out = open(name_out.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);

	default:
		break;
	}
}

//------------------------------------------------------------------------------

string		Method::temp_name()
{
	static unsigned int		count;
	string		name("./.temp");
	name += ft::itoa(count++);
	return name;
}

//------------------------------------------------------------------------------

char**		Method::make_meta_variable()
{
	char**	meta_var = new char*[18];
	string	meta_var_str[17];
	for (int i = 0 ; i < 16 ; i++)
	{
		meta_var_str[i].reserve(200);
	}
	meta_var_str[0] = string("AUTH_TYPE="			).append(location.auth_type);
	meta_var_str[1] = string("CONTENT_LENGTH="		).append(req.headers[CONTNET_LENGTH]);
	meta_var_str[2] = string("CONTENT_TYPE="		).append(req.headers[CONTNET_LENGTH]);
	meta_var_str[3] = string("GATEWAY_INTERFACE="	).append("CGI/1.1");
	meta_var_str[4] = string("PATH_INFO="			).append(req.path_info);
	meta_var_str[5] = string("PATH_TRANSLATED="		).append(req.path_translated);
	meta_var_str[6] = string("QUERY_STRING="		).append(req.query);
	meta_var_str[7] = string("REMOTE_ADDR="			).append("");
	// meta_var_str[7] = string("REMOTE_ADDR="			).append(ft::addr_to_str(ft::hton(sock.s_addr.sin_addr.s_addr)));
	meta_var_str[8] = string("REMOTE_IDENT="		).append(req.headers[AUTHORIZATION]);
	meta_var_str[9] = string("REMOTE_USER="			).append("");
	meta_var_str[10] = string("REQUEST_METHOD="		).append(Method::method_strings[req.method]);
	meta_var_str[11] = string("REQUEST_URI="		).append(req.path_info);
	meta_var_str[12] = string("SCRIPT_NAME="		).append(req.path_translated);
	meta_var_str[13] = string("SERVER_NAME="		).append(server.name);
	meta_var_str[14] = string("SERVER_PORT="		).append(ft::itoa(server.port));
	meta_var_str[15] = string("SERVER_PROTOCOL="	).append(req.protocol);
	meta_var_str[16] = string("SERVER_SOFTWARE="	).append("Webserver42/1.0.0");
	for (int i = 0 ; i < 16 ; i++)
	{
		meta_var[i] = ft::strdup(meta_var[i], meta_var_str[i].c_str());
	}
	meta_var[17] = 0;
	return meta_var;
}

//------------------------------------------------------------------------------

string&		Method::get_method(uint32_t num)
{
	return method_strings[num];
}

//------------------------------------------------------------------------------

uint32_t	Method::get_method(const string& method_name)
{
	return method_num[method_name];
}

//------------------------------------------------------------------------------

string		Method::get_allow(uint32_t flag)
{
	string	line("Allow: ");
	int		k = 0;
	for (int i = 0 ; flag ; ++i)
	{
		if (flag & 1)
		{
			if (k)
				line += ", ";
			line += get_method(i);
			k++;
		}
	}
	line += "\r\n";
	return line;
}

//------------------------------------------------------------------------------

void		Method::init_method_num()
{
	method_num["CONNECT"] = CONNECT;
	method_num["DELETE"] = DELETE;
	method_num["GET"] = GET;
	method_num["HEAD"] = HEAD;
	method_num["OPTIONS"] = OPTIONS;
	method_num["PATCH"] = PATCH;
	method_num["POST"] = POST;
	method_num["PUT"] = PUT;
	method_num["TRACE"] = TRACE;
}

//------------------------------------------------------------------------------

void		Method::init_method_flags()
{
	method_flags["CONNECT"] = (1 << CONNECT);
	method_flags["DELETE"] = (1 << DELETE);
	method_flags["GET"] = (1 << GET);
	method_flags["HEAD"] = (1 << HEAD);
	method_flags["OPTIONS"] = (1 << OPTIONS);
	method_flags["PATCH"] = (1 << PATCH);
	method_flags["POST"] = (1 << POST);
	method_flags["PUT"] = (1 << PUT);
	method_flags["TRACE"] = (1 << TRACE);
}

//------------------------------------------------------------------------------

void		Method::init_method_strings()
{
	method_strings[CONNECT] = string("CONNECT");
	method_strings[DELETE] = string("DELETE");
	method_strings[GET] = string("GET");
	method_strings[HEAD] = string("HEAD");
	method_strings[OPTIONS] = string("OPTIONS");
	method_strings[PATCH] = string("PATCH");
	method_strings[POST] = string("POST");
	method_strings[PUT] = string("PUT");
	method_strings[TRACE] = string("TRACE");
}
