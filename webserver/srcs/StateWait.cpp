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
	// cout  << __func__ << ": Wait" << endl;
	(void)ref;
	return waiting;
}