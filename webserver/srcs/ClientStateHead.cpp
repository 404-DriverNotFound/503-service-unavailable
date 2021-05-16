#include "../includes/ClientStateHead.hpp"

ClientStateHead::ClientStateHead()
{
}

ClientStateHead::~ClientStateHead()
{
}

ClientState* ClientStateHead::action(Client& ref)
{
	std::cout << "Head" << std::endl;
	return (ClientState*)ClientState::startline;
}
