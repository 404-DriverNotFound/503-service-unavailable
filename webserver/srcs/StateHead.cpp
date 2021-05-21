#include "../includes/StateHead.hpp"
#include "../includes/StateSet.hpp"

//------------------------------------------------

StateHead::StateHead()
{
	len = 0x2000;
}


//------------------------------------------------

StateHead::~StateHead()
{
}


//------------------------------------------------

State* StateHead::action(Client& ref)
{
	// cout << "Head : " << __func__ << endl;
	while (ref.get_httpreq().get_next_line())
	{
		if (ref.get_httpreq().get_string().empty())
		{
			return bind;
		}
		ref.get_httpreq().set_header(ref.get_httpreq().get_string());
	}
	return head;
}
