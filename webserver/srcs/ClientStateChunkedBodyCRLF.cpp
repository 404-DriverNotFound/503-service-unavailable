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
	if (ref.read_crlf())
	{
		return chunkedbody_len;
	}
	else
	{
		return this;
	}
}
