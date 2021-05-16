#pragma once

#include "ClientState.hpp"

class ClientStateMethod : public ClientState
{
	private:
	public:
		ClientStateMethod();
		~ClientStateMethod();
		ClientState*	action(Client& ref);
};
