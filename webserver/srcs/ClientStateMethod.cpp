#include "../includes/ClientStateMethod.hpp"
#include "../includes/ClientStateWait.hpp"

ClientStateMethod::ClientStateMethod()
{
}

ClientStateMethod::~ClientStateMethod()
{
}

ClientState* ClientStateMethod::action(Client& ref)
{
	const string&	method = ref.get_httpreq().get_method();

	if (method == "GET")
	{
		
	}
	else if (method == "HEAD")
	{
		
	}
	else if (method == "PUT")
	{

	}
	else if (method == "POST")
	{

	}
	else if (method == "DELETE")
	{
		
	}
	else
	{

	}
	return	waiting;
}