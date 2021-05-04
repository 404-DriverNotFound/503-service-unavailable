#include "../includes/Client.hpp"

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
		size_t len = req.stream.fill(0x2000);
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