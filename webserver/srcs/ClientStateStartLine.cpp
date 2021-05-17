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
	if (ref.get_httpreq().get_next_line())
	{
		try
		{
			ref.get_httpreq().set_start_line();
			return	head;
		}
		catch(int code)
		{
			ref.get_httpres().set_status_code(code);
			return head;
		}
	}
	return startline;
}
