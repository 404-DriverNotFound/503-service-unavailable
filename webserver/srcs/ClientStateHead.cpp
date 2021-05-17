#include "../includes/ClientStateHead.hpp"
#include "../includes/ClientStateSet.hpp"

//------------------------------------------------

ClientStateHead::ClientStateHead()
{
}


//------------------------------------------------

ClientStateHead::~ClientStateHead()
{
}


//------------------------------------------------

ClientState* ClientStateHead::action(Client& ref)
{
	while (ref.get_httpreq().get_next_line())
	{
		if (ref.get_httpreq().get_string().empty())
		{
			break;
		}
		ref.get_httpreq().set_header(ref.get_httpreq().get_string());
	}
	return bind;
}
