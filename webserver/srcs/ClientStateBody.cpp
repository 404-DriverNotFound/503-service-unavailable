#include "../includes/ClientStateBody.hpp"

/*===================
constructor
===================*/
ClientStateBody::ClientStateBody()
{
}

/*===================
destructor
===================*/
ClientStateBody::~ClientStateBody()
{
}

//--------------------
ClientState* ClientStateBody::action(Client& ref)
{
	std::cout << "Body" << std::endl;
	return (ClientState*)this->method;
}
