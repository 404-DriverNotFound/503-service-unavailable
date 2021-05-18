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
	if (ref.get_httpres().get_stream().fill(0xffff) == 0)
		return done;
	return waiting;
}