#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateMethod.hpp"
#include "../includes/Client.hpp"

/*===================
constructor
===================*/
ClientStateBody::ClientStateBody()
{
	len = 0xffff;
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
	cout << "Body : " <<  __func__ << endl;
	if (ref.read_chunked())
		return method;
	else
		return this;
}
