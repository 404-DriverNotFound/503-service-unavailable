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
	// cout << "Done : " <<  __func__ << endl;
	/*clear*/
	ref.clear();
	return startline;
	// return NULL;
}
