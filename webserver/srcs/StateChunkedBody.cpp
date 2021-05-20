#include "../includes/StateChunkedBody.hpp"
#include "../includes/StateChunkedBodyCRLF.hpp"
#include "../includes/StateWait.hpp"
#include "../includes/StateDone.hpp"

StateChunkedBody::StateChunkedBody()
{
	len = 0xffff;
}

StateChunkedBody::~StateChunkedBody()
{
}

State* StateChunkedBody::action(Client& ref)
{
	cout << "Chunked body : " << __func__ << endl;
	if (ref.read_chunked())
		return chunkedbody_crlf;
	else
		return this;
}
