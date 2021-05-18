#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateHead.hpp"

//---------------------------------------------------------

ClientStateStartLine::ClientStateStartLine()
{
	len = 0x2000;
}

//---------------------------------------------------------

ClientStateStartLine::~ClientStateStartLine()
{
}


//---------------------------------------------------------

ClientState* ClientStateStartLine::action(Client& ref)
{
	cout << "StartLine : " << __func__ << endl;
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
