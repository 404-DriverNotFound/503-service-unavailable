#include "../includes/StateChunkedBodyEnd.hpp"
#include "../includes/StateMethod.hpp"

StateChunkedBodyEnd::StateChunkedBodyEnd()
{
	len = 0xffffff;
}

StateChunkedBodyEnd::~StateChunkedBodyEnd()
{
}

State* StateChunkedBodyEnd::action(Client& ref)
{
	// cout << "chunked body end : " << __func__ << endl;
	if (ref.read_crlf())
	{
		return method;
	}
	else
	{
		return this;
	}
}
