#pragma once

#include "ClientState.hpp"

class ClientStateWait : public ClientState
{
	private:
	public:
		ClientStateWait();
		~ClientStateWait();
		ClientState*	action(Client& ref);
};