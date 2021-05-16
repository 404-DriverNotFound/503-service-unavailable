#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateHead.hpp"

//---------------------------------------------------------

ClientStateStartLine::ClientStateStartLine()
{
}

//---------------------------------------------------------

ClientStateStartLine::~ClientStateStartLine()
{
}


//---------------------------------------------------------

ClientState* ClientStateStartLine::action(Client& ref)
{
	if (ref.get_next_line())
	{
		ref.get_httpreq().set_start_line(ref.get_line());
		return	head;
	}
	return startline;
}
