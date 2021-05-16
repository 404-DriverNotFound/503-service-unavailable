#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateHead.hpp"

ClientStateStartLine::ClientStateStartLine()
{
}

ClientStateStartLine::~ClientStateStartLine()
{
}

ClientState* ClientStateStartLine::action(Client& ref)
{
	std::cout << "StartLine" << std::endl;
	return head;
}
