#include "../includes/StateChunkedBodyEnd.hpp"
#include "../includes/StateChunkedBodyLen.hpp"
#include "../includes/StateChunkedBody.hpp"
#include "../includes/StateMethod.hpp"

StateChunkedBodyLen::StateChunkedBodyLen()
{
	len = 0xffff;
}

StateChunkedBodyLen::~StateChunkedBodyLen()
{
}

State* StateChunkedBodyLen::action(Client& ref)
{
	cout << "chunked body len : " << __func__ << endl;
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
