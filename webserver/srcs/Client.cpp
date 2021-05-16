#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"

Client::Client(int accept_fd, map<string, ConfigServer>& ref)
: _socket(accept_fd), _servers(ref)
{
	_state = ClientState::startline;
}

Client::~Client()
{
}

void	Client::routine()
{
	_state = _state->action(*this);
}

