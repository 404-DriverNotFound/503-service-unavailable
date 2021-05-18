#include "../includes/ClientStateChunkedBodyEnd.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateMethod.hpp"

ClientStateChunkedBody::ClientStateChunkedBody()
{
	len = 0xffff;
}

ClientStateChunkedBody::~ClientStateChunkedBody()
{
}

ClientState* ClientStateChunkedBody::action(Client& ref)
{
	if (ref.set_chunked_length())
	{
		if (ref.get_httpreq().get_stream().get_pass_remain() > ref.get_location().get_body_max())
		{
			ref.get_httpres().set_status_code(413);
		}
		else if (ref.get_httpreq().get_stream().get_pass_remain() == 0)
		{
			return chunkend_end;
		}
		return chunkedbody;
	}
	else
	{
		return this;
	}
}
