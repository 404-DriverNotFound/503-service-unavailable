#pragma once

#include "ClientState.hpp"

class ClientStateDone : public ClientState
{
	private:
	public:
		ClientStateDone();
		~ClientStateDone();
		ClientState*	action(Client& ref);
};
