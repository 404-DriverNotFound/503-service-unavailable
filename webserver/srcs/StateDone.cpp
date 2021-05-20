#include "../includes/StateDone.hpp"
#include "../includes/StateStartLine.hpp"

StateDone::StateDone()
{
	len = 0xffff;
}

StateDone::~StateDone()
{
}

State* StateDone::action(Client& ref)
{
	cout << "Done : " <<  __func__ << endl;
	/*clear*/
	ref.update_birth();
	ref.get_httpreq().clear();
	ref.get_httpres().clear();
	return startline;
	// return NULL;
}
