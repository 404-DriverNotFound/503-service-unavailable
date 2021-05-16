#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"

// Client::Client(int accept_fd, map<string, ConfigServer>& ref)
// : _socket(accept_fd), _servers(ref)
// {
// 	_state = (ClientState*)ClientState::startline;
// }

Client::Client() {
	_state = (ClientState*)ClientState::startline;
}

Client::~Client()
{
}

void	Client::routine()
{
	_state = _state->action(*this);
}

HttpReq&	Client::getReq(void)
{
	// return (_req);
}

void		Client::setState(ClientState* _newState)
{
	_state = _newState;
}

int	main() {
	Client	A;

	A.routine();
	A.routine();
	A.routine();
}
