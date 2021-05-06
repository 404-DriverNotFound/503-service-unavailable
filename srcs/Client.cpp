#include "../includes/Client.hpp"
#include "../includes/MethodGet.hpp"
#include "../includes/MethodPost.hpp"
#include "../includes/MethodPut.hpp"
#include "../includes/MethodHead.hpp"

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
	try
	{
		routine();
	}
	catch(int code)
	{
		manage_err(code);
		status = CLIENT_SEND;
	}
}

//------------------------------------------------------------------------------

void			Client::routine()
{
	// usleep(400000);
	cout << "in process\n";
	if (is_expired())
		status = CLIENT_DONE;
	recv_stream();
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
			cout << "Method end!\n";
			status = CLIENT_SEND;
		}
		if (status == CLIENT_METHOD)
			break;
	case CLIENT_SEND:
		break;
	case CLIENT_DONE:
		break;
	default:
		break;
	}
	send_stream();
}

void			Client::manage_err(int code)
{
	res.stream.clear();
	res.status_code = code;
	res.stream << res.get_startline();
	res.stream << res.get_server();
	res.stream << res.get_content_length(7);
	res.stream << string("Content-Type: text/plain; charset=utf-8\r\n");
	res.stream << "\r\n";
	res.stream << string("Error\r\n");
	res.send_length = 0;
	res.msg_length = res.stream.size();
	status = CLIENT_SEND;
}

//------------------------------------------------------------------------------

bool			Client::is_expired()
{
	cout << "elapsed: " << (Time() - birth).get_time_sec() << endl;
	if ((Time() - birth).get_time_usec() > 30000000)
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
													cout << __func__ << endl;
		size_t	len = 0;
		switch (status)
		{
			case CLIENT_STARTLINE:
				len = req.stream.fill(0x2000);
				break;
			case CLIENT_HEADER:
				len = req.stream.fill(0x2000);
				break;
			default:
				len = req.stream.fill(0x10000);
				break;
		}
		cout << "- input len: " << len << endl;
		if (len == 0)
		{
			cout << "\n--- Client Done! ---" << endl;
			status = CLIENT_DONE;
		}
		r_set.del(sock.fd);
		
		
		cout << "\n--- request ---" << endl;
		req.stream.print_line();
		cout << "\n--- request ---" << endl;
	}
}

//------------------------------------------------------------------------------

void			Client::send_stream()
{
	if (w_set.get(sock.fd))
	{
		
		cout << __func__ << endl;
		cout << "\n--- response ---" << endl;
		res.stream.print_line();
		cout << "\n--- response ---" << endl;
		cout << "send: " << res.send_length << " / " << res.msg_length << endl;

		
		res.send_length += res.stream.pass();

		
		cout << "send: " << res.send_length << " / " << res.msg_length << endl;


		
		if (res.msg_length && res.msg_length <= res.send_length)
		{
			cout << "\n--- Send Done! ---" << endl;
			reset();
			// usleep(1000000);
			// status = CLIENT_DONE;
		}
		w_set.del(sock.fd);
	}
}

void			Client::reset()
{
	cout << __func__ << endl;
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
	cout << __func__ << endl;
	if (req.stream.get_line(req.line))
	{
		req.set_start_line(req.line);
		status = CLIENT_HEADER;
	}
}

//------------------------------------------------------------------------------

void			Client::set_request_header()
{
	cout << __func__ << endl;
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
	cout << __func__ << endl;
	string				host;
	string::const_iterator	it = req.headers[HOST].begin();
	ft::get_chr_token(req.headers[HOST], it, host, ':');
	iterator_server	it_server = servers.find(host);
	if (it_server == servers.end())
	{
		cout << "server not found: " << host << endl;
		throw 404;
	}
	server = &it_server->second;
}

//------------------------------------------------------------------------------

void		Client::set_location()
{
	cout << __func__ << endl;
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
	cout << __func__ << endl;
	if (location->auth.empty())
		return ;

	string&					line_auth = req.headers[AUTHORIZATION];
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
	cout << __func__ << endl;
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
	cout << __func__ << endl;
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
	cout << __func__ << endl;
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
