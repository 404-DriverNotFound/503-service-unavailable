#include "../includes/ClientStateChunkedBodyEnd.hpp"
#include "../includes/ClientStateChunkedBodyLen.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateMethod.hpp"

ClientStateChunkedBodyLen::ClientStateChunkedBodyLen()
{
	len = 0xffff;
}

ClientStateChunkedBodyLen::~ClientStateChunkedBodyLen()
{
}

ClientState* ClientStateChunkedBodyLen::action(Client& ref)
{
	cout << "body len : " << __func__ << endl;
	if (ref.set_chunked_length())
	{
		if (ref.get_httpreq().get_stream().get_pass_remain() > ref.get_location().get_body_max())
		{
			ref.get_httpres().set_status_code(413);
		}
		else if (ref.get_httpreq().get_stream().get_pass_remain() == 0)
		{
			return chunkedbody_end;
		}
		return chunkedbody;
	}
	else
	{
		return this;
	}
}
