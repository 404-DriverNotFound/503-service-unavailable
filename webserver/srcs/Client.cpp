#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"

Client::Client(int accept_fd, ServerMap& ref)
: _socket(accept_fd),
  _servers(ref),
  _state(ClientState::startline)
{
}

Client::~Client()
{
}

void	Client::routine()
{
	_state = _state->action(*this);
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
