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
	while (ref.get_next_line())
	{
		if (ref.get_line().empty())
		{
			break;
		}
		ref.get_httpreq().set_header(ref.get_line());
	}
	return set;
}
