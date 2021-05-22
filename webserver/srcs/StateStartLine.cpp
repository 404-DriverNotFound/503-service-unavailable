#include "../includes/StateStartLine.hpp"
#include "../includes/StateHead.hpp"

//---------------------------------------------------------

StateStartLine::StateStartLine()
{
	len = 0x2000;
}

//---------------------------------------------------------

StateStartLine::~StateStartLine()
{
}


//---------------------------------------------------------

State* StateStartLine::action(Client& ref)
{
	// cout << __func__ << ": StartLine" << endl;
	if (ref.get_httpreq().get_next_line())
	{
		ref.get_httpreq().set_start_line();
		return	head;
	}
	return startline;
}
