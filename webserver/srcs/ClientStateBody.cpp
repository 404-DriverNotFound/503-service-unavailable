#include "../includes/ClientStateBody.hpp"

ClientStateBody::ClientStateBody()
{
}

ClientStateBody::~ClientStateBody()
{
}

ClientState* ClientStateBody::action(Client& ref)
{
	std::cout << "Body" << std::endl;
	return (ClientState*)this->method;
}
