#include "../includes/ClientStateDone.hpp"
#include "../includes/ClientStateStartLine.hpp"

ClientStateDone::ClientStateDone()
{
	len = 0xffff;
}

ClientStateDone::~ClientStateDone()
{
}

ClientState* ClientStateDone::action(Client& ref)
{
	cout << "Done : " <<  __func__ << endl;
	/*clear*/
	ref.update_birth();
	ref.get_httpreq().clear();
	ref.get_httpres().clear();
	return startline;
	// return NULL;
}
