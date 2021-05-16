#include "../includes/ClientStateDone.hpp"

ClientStateDone::ClientStateDone()
{
}

ClientStateDone::~ClientStateDone()
{
}

ClientState* ClientStateDone::action(Client& ref)
{
	std::cout << "Done" << std::endl;
	return NULL;
}
