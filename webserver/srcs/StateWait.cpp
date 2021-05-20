#include "../includes/StateWait.hpp"
#include "../includes/StateDone.hpp"

StateWait::StateWait()
{
}

StateWait::~StateWait()
{
}

State*	StateWait::action(Client& ref)
{
	cout << "Wait" << __func__ << endl;
	return waiting;
}