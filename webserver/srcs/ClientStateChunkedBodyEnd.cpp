#include "../includes/ClientStateChunkedBodyEnd.hpp"
#include "../includes/ClientStateMethod.hpp"

ClientStateChunkedBodyEnd::ClientStateChunkedBodyEnd()
{
	len = 0xffff;
}

ClientStateChunkedBodyEnd::~ClientStateChunkedBodyEnd()
{
}

ClientState* ClientStateChunkedBodyEnd::action(Client& ref)
{
	cout << "body end : " << __func__ << endl;
	if (ref.read_crlf())
	{
		return method;
	}
	else
	{
		return this;
	}
}
