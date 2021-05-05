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
res(sock.fd)
{
	status = CLIENT_STARTLINE;
}

//------------------------------------------------------------------------------

/*destructor*/	Client::~Client()
{
}

//------------------------------------------------------------------------------

void			Client::process()
{
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
			status = CLIENT_SEND;
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

//------------------------------------------------------------------------------

bool			Client::is_expired()
{
	if ((Time() - birth).get_time_usec() > location->timeout)
		status = CLIENT_DONE;
}

//------------------------------------------------------------------------------

void			Client::recv_stream()
{
	if (r_set.get(sock.fd))
	{
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
		if (len == 0)
		{
			status = CLIENT_DONE;
		}
		r_set.del(sock.fd);
	}
}

//------------------------------------------------------------------------------

void			Client::send_stream()
{
	if (w_set.get(sock.fd))
	{
		res.stream.pass();
		w_set.del(sock.fd);
	}
}

//------------------------------------------------------------------------------

void			Client::set_request_startline()
{
	if (req.stream.get_line(req.line))
	{
		cout << req.line << endl;
		req.set_start_line(req.line);
		status = CLIENT_HEADER;
	}
}

//------------------------------------------------------------------------------

void			Client::set_request_header()
{
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
		cout << req.method << endl;
		cout << location->method << endl;
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
	set_server();
	set_location();
	check_auth();
	set_path();
	set_method();
	switch (req.method)
	{
		case GET:
			method = new MethodGet(req, res, server, location);
		case POST:
			method = new MethodPost(req, res, server, location);
		break;
	
	default:
		break;
	}
	method
}