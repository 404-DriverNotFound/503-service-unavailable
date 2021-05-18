#include "../includes/ClientStateHead.hpp"
#include "../includes/ClientStateSet.hpp"

//------------------------------------------------

ClientStateHead::ClientStateHead()
{
	len = 0x2000;
}


//------------------------------------------------

ClientStateHead::~ClientStateHead()
{
}


//------------------------------------------------

ClientState* ClientStateHead::action(Client& ref)
{
	cout << "Head : " << __func__ << endl;
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
