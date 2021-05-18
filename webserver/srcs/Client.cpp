#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateDone.hpp"

Client::Client(int accept_fd, ServerMap& ref, FdSet& r_set, FdSet& w_set)
: _socket(accept_fd),
  _servers(ref),
  _r_set(r_set),
  _w_set(w_set),
  _state(ClientState::startline),
  _req(_socket.fd),
  _res(_socket.fd)
{
}

Client::~Client()
{
}

void	Client::routine()
{
	recv_socket(_state->len);
	_state = _state->action(*this);
	send_socket(_state->len);
}

/*=======================
getter
=======================*/
Socket&				Client::get_socket()
{
	return	_socket;
}

Time&				Client::get_time()
{
	return	_birth;
}

Client::ServerMap&	Client::get_servers()
{
	return	_servers;
}

const ConfigServer&		Client::get_server()
{
	return	*_server;
}

const ConfigLocation&	Client::get_location()
{
	return	*_location;
}

HttpReq&			Client::get_httpreq()
{
	return	_req;
}

HttpRes&			Client::get_httpres()
{
	return	_res;
}

ClientState*		Client::get_clientstate()
{
	return	_state;
}

bool				Client::get_next_line()
{
	return _req.get_next_line();
}

bool				Client::set_chunked_length()
{
	return _req.set_chunked_length();
}

/*=======================
setter
=======================*/
void				Client::set_server(const ConfigServer& svrp)
{
	_server = &svrp;
}

void				Client::set_location(const ConfigLocation& locp)
{
	_location = &locp;
}

void				Client::recv_socket(size_t len)
{
	if(_req.get_stream().fill(len) == 0)
	{
		_state = ClientState::done;
	}
}

void				Client::send_socket(size_t len)
{
	_res.get_stream().pass();
}