#include "../includes/ClientStateMethod.hpp"

ClientStateMethod::ClientStateMethod()
{
}

ClientStateMethod::~ClientStateMethod()
{
}

ClientState* ClientStateMethod::action(Client& ref)
{
	std::cout << "Method" << std::endl;
	return (ClientState*)NULL;
}

