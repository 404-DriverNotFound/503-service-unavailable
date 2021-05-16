#include "../includes/ClientStateStartLine.hpp"

ClientStateStartLine::ClientStateStartLine() : ClientState()
{
}

ClientStateStartLine::~ClientStateStartLine()
{
}

ClientState* ClientStateStartLine::action(Client& ref)
{
	std::cout << "StartLine" << std::endl;
	return (ClientState*)ClientState::head;
}
