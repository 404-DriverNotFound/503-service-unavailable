#include "../includes/ClientStateWait.hpp"
#include "../includes/ClientStateDone.hpp"

ClientStateWait::ClientStateWait()
{
}

ClientStateWait::~ClientStateWait()
{
}

ClientState*	ClientStateWait::action(Client& ref)
{
	return waiting;
}