#include "../includes/HttpReq.hpp"
#include "../includes/Client.hpp"

Client::Client(int accept_fd, map<string, ConfigServer>& ref)
: _socket(accept_fd), _servers(ref)
{
	_state = &ClientState::startline;
}

Client::~Client()
{
	if (_method)
		delete _method;
}

void	Client::routine()
{
	_state->done();
}

HttpReq&	Client::getReq(void)
{
	return (_req);
}