#include "../includes/ClientStateHead.hpp"
#include "../includes/ClientStateSet.hpp"

ClientStateHead::ClientStateHead()
{
}

ClientStateHead::~ClientStateHead()
{
}

ClientState* ClientStateHead::action(Client& ref)
{
	std::cout << "Head" << std::endl;
	return set;
}
