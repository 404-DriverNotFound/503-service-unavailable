#include "../includes/StateChunkedBodyCRLF.hpp"
#include "../includes/StateChunkedBodyLen.hpp"

StateChunkedBodyCRLF::StateChunkedBodyCRLF()
{
	len = 0xffff;
}

StateChunkedBodyCRLF::~StateChunkedBodyCRLF()
{
}

State* StateChunkedBodyCRLF::action(Client& ref)
{
	cout << "chunked body crlf : " << __func__ << endl;
	if (ref.read_crlf())
	{
		return chunkedbody_len;
	}
	else
	{
		return this;
	}
}
