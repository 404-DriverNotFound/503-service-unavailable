#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateChunkedBodyCRLF.hpp"
#include "../includes/ClientStateWait.hpp"
#include "../includes/ClientStateDone.hpp"

ClientStateChunkedBody::ClientStateChunkedBody()
{
	len = 0xffff;
}

ClientStateChunkedBody::~ClientStateChunkedBody()
{
}

ClientState* ClientStateChunkedBody::action(Client& ref)
{
	if (ref.read_chunked())
		return chunkedbody_crlf;
	else
		return this;
}
