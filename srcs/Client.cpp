#include "../includes/Client.hpp"
#include "../includes/MethodGet.hpp"
#include "../includes/MethodPost.hpp"
#include "../includes/MethodPut.hpp"
#include "../includes/MethodHead.hpp"

#define TIMEOUT 30000000

// #define DBG

//------------------------------------------------------------------------------

/*constructor*/	Client::Client(int accept_fd, map<string, Server>& servers, 
				FdSet& r_set, FdSet& w_set)
: sock(accept_fd), 
servers(servers), 
r_set(r_set),
w_set(w_set),
birth(),
req(sock.fd),
res(sock.fd),
method(0)
{
	status = CLIENT_STARTLINE;
	client_status = STATUS_ROUTINE;

	// cout << servers["localhost"].temp_file_dir << endl;
}

//------------------------------------------------------------------------------

/*destructor*/	Client::~Client()
{
	if (method)
		delete method;
}

//------------------------------------------------------------------------------

void			Client::process()
{
	#ifdef DBG
	cout << "in process\n";
	#endif
	// if (is_expired())
	// 	status = CLIENT_DONE;
	recv_stream();
	try
	{
		routine();
	}
	catch(int code)
	{
		manage_err(code);
	}
	
}

//------------------------------------------------------------------------------

void			Client::routine()
{
	// usleep(400000);
	#ifdef DBG
	cout << "status: " << status << endl;
	#endif

	switch (status)
	{
	case CLIENT_STARTLINE:
		set_request_startline();
		if (status == CLIENT_STARTLINE)
			break;
	case CLIENT_HEADER:
		set_request_header();
		if (status == CLIENT_HEADER)
			break;
	case CLIENT_SET:
		set_client();
	case CLIENT_METHOD:
		if (method->run())
		{
			#ifdef DBG
			cout << "Method end!\n";
			#endif
			status = CLIENT_SEND;
		}
		if (status == CLIENT_METHOD)
			break;
	case CLIENT_SEND:
		send_stream();
		break;
	case CLIENT_DONE:
		break;
	case CLIENT_ERR:
		err();
		if (status == CLIENT_ERR)
			break;
	default:
		break;
	}
}

void			Client::manage_err(int code)
{
	#ifdef DBG
	cout <<__func__ << endl;
	#endif

	if (code == 400)
		return ;

	// res.stream.clear();
	res.status_code = code;
	res.stream << res.get_startline();
	res.stream << res.get_server();
	res.stream << res.get_content_length(7);
	// TODO: 에러페이지 타입을 잘 적자
	res.stream << string("Content-Type: text/plain; charset=utf-8\r\n");
	res.stream << "\r\n";
	// TODO: 에러페이지를 찾아서 전송하기
	res.stream << string("Error\r\n");
	res.send_length = 0;
	res.msg_length = res.stream.size();
	status = CLIENT_ERR;
	// status = CLIENT_SEND;
	// client_status = STATUS_DEAD;
}

void			Client::err()
{
	#ifdef DBG
	cout <<__func__ << endl;
	#endif
	if (req.headers["TRANSFER_ENCODING"] == "chunked")
	{
		if (req.stream.get_seq_token(req.line, "0\r\n\r\n"))
		{
			#ifdef DBG
			cout << "find: 0rnrn\n";
			#endif
			// if (req.line == "0")
				status = CLIENT_SEND;
		}
		else
		{
			#ifdef DBG
			cout << req.line << endl;
			#endif
		}
	}
	else
	{
		status = CLIENT_SEND;
	}
	
}

//------------------------------------------------------------------------------

bool			Client::is_expired()
{
	#ifdef DBG
	cout << "elapsed: " << (Time() - birth).get_time_sec() << endl;
	#endif
	if ((Time() - birth).get_time_usec() > TIMEOUT)
	{
		status = CLIENT_DONE;
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------

void			Client::recv_stream()
{
	if (r_set.get(sock.fd))
	{
		#ifdef DBG
		cout << __func__ << endl;
		#endif

		ssize_t	len = 0;
		switch (status)
		{
			case CLIENT_STARTLINE:
				len = req.stream.fill(0x2000);
				break;
			case CLIENT_HEADER:
				len = req.stream.fill(0x2000);
				break;
			default:
				len = req.stream.fill(req.stream.default_capacity);
				break;
		}
		#ifdef DBG
		cout << "- input len: " << len << endl;
		#endif
		if (len == 0)
		{
			#ifdef DBG
			cout << "\n--- Client Done! ---" << endl;
			#endif

			status = CLIENT_DONE;
		}
		else if (len < 0)
		{
			#ifdef DBG
			cout << "\n--- Client ERR! ---" << endl;
			#endif

			status = CLIENT_DONE;
		}
		r_set.del(sock.fd);
		
		#ifdef DBG
		cout << "\n--- request ---" << endl;
		if (req.stream.size() < 1000)
			req.stream.print_line();
		else
			cout << "len: " << req.stream.size() << endl;
		cout << "\n--- request ---" << endl;
		#endif
	}
}

//------------------------------------------------------------------------------

void			Client::send_stream()
{
	if (w_set.get(sock.fd))
	{
		#ifdef DBG
		cout << __func__ << endl;
		cout << "\n--- response ---" << endl;
		if (res.stream.size() < 1000)
			res.stream.print_line();
		else
			cout << "len: " << (res.stream.buffers.front().end - res.stream.it_buffer) << endl;
		cout << "\n--- response ---" << endl;
		cout << "send: " << res.send_length << " / " << res.msg_length << endl;
		#endif
		
		
		res.send_length += res.stream.pass();

		
		#ifdef DBG
		cout << "send: " << res.send_length << " / " << res.msg_length << endl;
		#endif

		
		if (res.msg_length && res.msg_length <= res.send_length)
		{
			#ifdef DBG
			cout << "\n--- Send Done! ---" << endl;
			#endif

			// if (client_status == STATUS_DEAD)// && req.method != POST)
			// 	status = CLIENT_DONE;
			// else
				reset();
		}
		w_set.del(sock.fd);
	}
}

void			Client::reset()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	client_status = STATUS_ROUTINE;
	birth.set_current();
	status = CLIENT_STARTLINE;
	req.clear();
	res.clear();
	server = 0;
	location = 0;
	delete method;
	method = 0;
}

//------------------------------------------------------------------------------

void			Client::set_request_startline()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	if (req.stream.get_line(req.line))
	{
		req.set_start_line(req.line);
		status = CLIENT_HEADER;
	}
}

//------------------------------------------------------------------------------

void			Client::set_request_header()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	while (req.stream.get_line(req.line))
	{
		if (req.line.empty())
		{
			status = CLIENT_SET;
			break;
		}
		req.set_header(req.line);
	}
}

//------------------------------------------------------------------------------

void		Client::set_server()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	// 헤더에서 호스트 찾기
	map<string, string>::iterator	it_header = req.headers.find("HOST");
	if (it_header == req.headers.end())
	{
		cout << "Host Empty\n";
		throw 400;
	}
	// 호스트에서 포트 제거
	string					host;
	string::const_iterator	it_string = it_header->second.begin();
	ft::get_chr_token(it_header->second, it_string, host, ':');
	// 호스트로 서버 찾기
	iterator_server			it_server = servers.find(host);
	if (it_server == servers.end())
	{
		cout << "Server Not Found\n";
		throw 404;
	}
	server = &it_server->second;
}

//------------------------------------------------------------------------------

void		Client::set_location()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	iterator_location	it_location	= server->locations.find("/" + req.path.front());
	if (it_location == server->locations.end())
	{
		if ((it_location = server->locations.find("/")) == server->locations.end())
			throw 404;
	}
	else
	{
		req.path.pop_front();
	}
	location = &it_location->second;
}

//------------------------------------------------------------------------------

void		Client::check_auth()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	if (location->auth.empty())
		return ;

	string&					line_auth = req.headers["AUTHORIZATION"];
	string::const_iterator	it = line_auth.begin();
	string					client_auth_type;
	string					client_auth;

	ft::get_chr_token(line_auth, it, client_auth_type, ' ');
	if (location->auth_type != client_auth_type)
		throw 401;	// TODO: bad request or unauthorized?
	ft::get_chr_token(line_auth, it, client_auth, ' ');
	if (location->auth != client_auth)
		throw 401;	// TODO: bad request or unauthorized?
}

//------------------------------------------------------------------------------

void		Client::set_method()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	if (!(location->method & (1 << req.method)))
	{
		cout << "Method Not allowed!" << endl;
		cout << Method::method_strings[req.method] << endl;
		cout << Method::get_allow(location->method) << endl;
		throw 405; // Method not allowed
	}
}

//------------------------------------------------------------------------------

void		Client::set_path()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	req.path_translated = location->root;
	list<string>::iterator 	it = req.path.begin();
	list<string>::iterator 	end = req.path.end();

	while (it != end)
	{
		req.path_translated.append("/");
		req.path_translated.append(*it);
		++it;
	}
}

//------------------------------------------------------------------------------

void		Client::set_client()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
	set_server();
	set_location();
	check_auth();
	set_path();
	set_method();
	switch (req.method)
	{
		case GET:
			method = new MethodGet(req, res, *server, *location);
			break;
		case PUT:
			method = new MethodPut(req, res, *server, *location);
			break;
		case POST:
			method = new MethodPost(req, res, *server, *location);
			break;
		break;
	
	default:
		break;
	}
	status = CLIENT_METHOD;
}
