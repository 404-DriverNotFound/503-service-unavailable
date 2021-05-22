#include "../includes/StateBody.hpp"
#include "../includes/StateMethod.hpp"
#include "../includes/Client.hpp"

/*===================
constructor
===================*/
StateBody::StateBody()
{
	len = 0xffff;
}

/*===================
destructor
===================*/
StateBody::~StateBody()
{
}

//--------------------
State* StateBody::action(Client& ref)
{
	// cout <<  __func__  << ": recv body" << endl;
	if (ref.read_chunked())
		return method;
	else
		return this;
}
