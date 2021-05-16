#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"

Client::Client(int accept_fd, ServerMap& ref)
: _socket(accept_fd),
  _stream(0xffffff, accept_fd, accept_fd),
  _servers(ref),
  _state(ClientState::startline)
{
}

Client::~Client()
{
}

void	Client::routine()
{
	_stream.fill(0xfffff);
	_state = _state->action(*this);
}

/*=======================
getter
=======================*/
Socket&				Client::get_socket()
{
	return	_socket;
}

Stream&				Client::get_stream()
{
	return	_stream;
}

Time&				Client::get_time()
{
	return	_birth;
}

Client::ServerMap&	Client::get_servers()
{
	return	_servers;
}

Path&				Client::get_path()
{
	return	_path;
}

ConfigServer&		Client::get_server()
{
	return	*_server;
}

ConfigLocation&		Client::get_location()
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

string&				Client::get_line()
{
	return	_line;
}

bool				Client::get_next_line()
{
	return get_stream().get_line(_line);
}
