#include "../includes/ClientStateDone.hpp"
#include "../includes/ClientStateStartLine.hpp"

ClientStateDone::ClientStateDone()
{
}

ClientStateDone::~ClientStateDone()
{
}

ClientState* ClientStateDone::action(Client& ref)
{
	/*clear*/
	ref.update_birth();
	ref.get_httpreq().clear();
	ref.get_httpres().clear();
	return startline;
	// return NULL;
}
