#include "../includes/Method.hpp"

// #define DBG

map<string, uint32_t>	Method::method_num;
map<string, uint32_t>	Method::method_flags;
string					Method::method_strings[NUM_METHOD];

//------------------------------------------------------------------------------

		Method::Method(HttpReq& req, HttpRes& res, Server& server, Location& location)
:req(req), res(res), cgi(0), server(server), location(location), fd_in(-1), fd_out(-1),
open_option(OPEN_GET)
{
	// lllen = 0;
}

//------------------------------------------------------------------------------

		Method::~Method()
{
	if (cgi)
		delete cgi;
	if (fd_in >= 0)
		close(fd_in);
	if (fd_out >= 0)
		close(fd_out);
	if (open_option == OPEN_POST_CGI || open_option == OPEN_POST_DIR)
		ft::rm_df(name_in.c_str());
	if (open_option == OPEN_POST_CGI)
		ft::rm_df(name_out.c_str());
}

//------------------------------------------------------------------------------

bool	Method::recv_body()
{
	if (req.content_length > location.body_length)
		throw 413;
	return !req.stream.pass(req.stream.pass_remain);
}

//------------------------------------------------------------------------------

bool	Method::run()
{
	
	#ifdef DBG
	cout << __func__ << endl;
	#endif
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
			status = METHOD_LOAD_BODY;
			goto METHOD_LOAD_BODY;
			
		case METHOD_LOAD_HEADER_CGI:
		METHOD_LOAD_HEADER_CGI:
			load_cgi_header();
			status = METHOD_LOAD_BODY;

		case METHOD_LOAD_BODY:
		METHOD_LOAD_BODY:
			load_body();
			if (status == METHOD_LOAD_BODY)
				break;

		case METHOD_DONE:
		METHOD_DONE:
			// cout << "Method Done!" << endl;
			return true;

		default:
			break;
	}
	return false;
}

//------------------------------------------------------------------------------

bool	Method::recv_chunked_body()
{
	
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	while (42)
	{
		switch (status_chunked)
		{
			case CHUNKED_SIZE:
				// cout << "- size: ";
				if (req.stream.get_line(req.line))
				{
					// cout << req.line << endl;
					req.stream.pass_remain = ft::atoi_hex(req.line);
					req.recv_length += req.stream.pass_remain;
					if (req.recv_length > location.body_length)
					{
						throw 413;
					}
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
					#ifdef DBG
					cout << req.line << endl;
					#endif
					return false;
				}
			case CHUNKED_RECV:
				// cout << "- send: " << req.stream.pass_remain << endl;
				req.stream.pass(req.stream.pass_remain);
				// cout << "- send: " << req.stream.pass_remain << endl;
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
				break;
			default:
				// cout << "status chunked: " <<  status_chunked << endl;
				break;
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

void	Method::load_cgi_header()
{
	
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	// close(fd_out);
	// fd_out = open(name_out.c_str(), O_RDONLY);
	// res.stream.fd_in = fd_out;


	Stream	stream(location.head_length, fd_out);
	string	line;

	stream.fill(location.head_length);
	size_t	cgi_header_size = stream.size();
	while (stream.get_line(line))
	{
		if (line.empty())
			break ;
		res.set_header(line);
	}
	cgi_header_size -= stream.size();
	res.content_length = ft::file_size(name_out.c_str()) - cgi_header_size;
	load_response_header();
	res.msg_length = res.stream.size();
	res.msg_length += res.content_length;
	
	// lllen += stream.size();
	res.stream.write(stream.it_buffer, stream.size());
}

//------------------------------------------------------------------------------

// void	Method::load_response_header()
// {
// 	// cout << "Method::" << __func__ << endl;
// 	// res.status_code = 200;
// 	// res.stream << res.get_startline();
// 	// res.stream << res.get_content_length();
// 	// res.stream << res.get_server();
// 	// res.stream << "\r\n";
// }

//------------------------------------------------------------------------------

void	Method::load_body()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	if (fd_out < 0)
	{
		status = METHOD_DONE;
		return ;
	}
	ssize_t	len = res.stream.fill(res.content_length < res.stream.default_capacity ? res.content_length : res.stream.default_capacity);
	// lllen += len;
	if (len == 0)
	{
		// cout << "- fill done!" << endl;
		// cout << "len: " << lllen << endl;
		// char	buff[10];
		// read(0, buff, 1);

		status = METHOD_DONE;
	}
}

//------------------------------------------------------------------------------

void		Method::open_file_base(const string& path)
{
	#ifdef DBG
	cout << "path: " << req.path_translated << endl;
	#endif
	if (ft::is_dir(path.c_str()))
	{
		string	path_tmp = ft::find(path, location.index);
		if (path_tmp.empty())
			throw 404;
		req.path_translated.append("/");
		req.path_translated.append(path_tmp);
		#ifdef DBG
		cout << "path(index): " << req.path_translated << endl;
		#endif
	}
}

//------------------------------------------------------------------------------

void		Method::open_file(e_openfile option)
{
	#ifdef DBG
	cout << __func__ << ": ";
	#endif
	switch (option)
	{
	case OPEN_GET:
		open_file_base(req.path_translated);
		fd_out = open(req.path_translated.c_str(), O_RDONLY);
		if (fd_out < 0)
			throw 404;
		res.stream.fd_in = fd_out;
		name_out = req.path_translated;
		break;

	case OPEN_PUT:
		if (ft::is_dir(req.path_translated.c_str()))
			throw 404;
		fd_in = open(req.path_translated.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (fd_in < 0)
			throw 500;
		#ifdef DBG
		cout << "path: " << req.path_translated << endl;
		#endif
		req.stream.fd_out = fd_in;
		name_in = req.path_translated;
		break;

	case OPEN_POST:
		fd_in = open(req.path_translated.c_str(), O_CREAT | O_APPEND | O_RDWR, 0644);
		req.stream.fd_out = fd_in;
		res.stream.fd_in = fd_out;
		break;

	case OPEN_POST_DIR:
		open_file_base(req.path_translated);
		res.content_length = ft::file_size(req.path_translated.c_str());
		name_in = temp_name();
		name_out = req.path_translated;
		fd_in = open(name_in.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		fd_out = open(req.path_translated.c_str(), O_RDONLY);
		if (fd_out < 0)
			throw 404;
		req.stream.fd_out = fd_in;
		res.stream.fd_in = fd_out;
		if (fd_in < 0 || fd_out < 0)
		{
			#ifdef DBG
			cout << "FDERR!\n";
			#endif
			exit(1);
		}
		break;
		

	case OPEN_POST_CGI:
		if (ft::is_dir(req.path_translated.c_str()))
			throw 404;
		name_in = temp_name();
		name_out = temp_name();
		fd_in = open(name_in.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		fd_out = open(name_out.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0644);
		req.stream.fd_out = fd_in;
		res.stream.fd_in = fd_out;	
		if (fd_in < 0 || fd_out < 0)
		{
			#ifdef DBG
			cout << "FDERR!\n";
			#endif
			exit(1);
		}

	default:
		break;
	}
}

//------------------------------------------------------------------------------

string		Method::temp_name()
{
	static unsigned int		count;
	// string		name("/Users/minckim/goinfre/temp");
	string		name("./root/temp");
	name += ft::itoa(count++);
	return name;
}

//------------------------------------------------------------------------------

void		Method::make_meta_variable()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif

	cgi->meta_variables.reserve(17 + req.headers.size());
	cgi->meta_variables.assign(17, string());

	cgi->meta_variables[0] = string("AUTH_TYPE="		).append(location.auth_type);
	cgi->meta_variables[1] = string("CONTENT_LENGTH="	).append(req.headers["CONTNET_LENGTH"]);
	cgi->meta_variables[2] = string("CONTENT_TYPE="		).append(req.headers["CONTNET_TYPE"]);
	cgi->meta_variables[3] = string("GATEWAY_INTERFACE=").append("CGI/1.1");
	cgi->meta_variables[4] = string("PATH_INFO="		).append(req.path_info);
	cgi->meta_variables[5] = string("PATH_TRANSLATED="	).append(req.path_translated);
	cgi->meta_variables[6] = string("QUERY_STRING="		).append(req.query);
	cgi->meta_variables[7] = string("REMOTE_ADDR="		).append("");
	// cgi->meta_variables[7] = string("REMOTE_ADDR="		).append(ft::addr_to_str(ft::hton(sock.s_addr.sin_addr.s_addr)));
	cgi->meta_variables[8] = string("REMOTE_IDENT="		).append(req.headers["AUTHORIZATION"]);
	cgi->meta_variables[9] = string("REMOTE_USER="		).append("");
	cgi->meta_variables[10] = string("REQUEST_METHOD="	).append(Method::method_strings[req.method]);
	cgi->meta_variables[11] = string("REQUEST_URI="		).append(req.path_info);
	cgi->meta_variables[12] = string("SCRIPT_NAME="		).append(req.path_translated);
	cgi->meta_variables[13] = string("SERVER_NAME="		).append(server.name);
	cgi->meta_variables[14] = string("SERVER_PORT="		).append(ft::itoa(server.port));
	cgi->meta_variables[15] = string("SERVER_PROTOCOL="	).append(req.protocol);
	cgi->meta_variables[16] = string("SERVER_SOFTWARE="	).append("Webserver42/1.0.0");

	map<string, string>::iterator	it_header = req.headers.begin();
	map<string, string>::iterator	it_header_end = req.headers.end();

	while (it_header != it_header_end)
	{
		cgi->meta_variables.push_back(string());
		cgi->meta_variables.back().reserve(100);
		cgi->meta_variables.back() += "HTTP_";
		cgi->meta_variables.back() += it_header->first;
		cgi->meta_variables.back() += "=";
		cgi->meta_variables.back() += it_header->second;
		++it_header;
	}

	// for (vector<string>::iterator it = cgi->meta_variables.begin(); it != cgi->meta_variables.end() ; ++it)
	// {
	// 	cout << *it << endl;
	// }
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
		flag >>= 1;
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

// int			main()
// {
// 	Method::init_method_num();
// 	Method::init_method_flags();
// 	Method::init_method_strings();
// 	cout << Method::get_allow(0) << endl;
// }