#include "../includes/ClientStateChunkedBodyCRLF.hpp"
#include "../includes/ClientStateChunkedBodyLen.hpp"

ClientStateChunkedBodyCRLF::ClientStateChunkedBodyCRLF()
{
	len = 0xffff;
}

ClientStateChunkedBodyCRLF::~ClientStateChunkedBodyCRLF()
{
}

ClientState* ClientStateChunkedBodyCRLF::action(Client& ref)
{
	cout << "body crlf : " << __func__ << endl;
	if (ref.read_crlf())
	{
		return chunkedbody_len;
	}
	else
	{
		return this;
	}
}
