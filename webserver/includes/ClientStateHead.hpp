#pragma once

#include "ClientState.hpp"

class ClientStateHead : public ClientState
{
	private:
	public:
		ClientStateHead();
		~ClientStateHead();
		ClientState*	action(Client& ref);
};
