#pragma once

#include "ClientState.hpp"

class ClientStateBody : public ClientState
{
	private:
	public:
		ClientStateBody();
		~ClientStateBody();
		ClientState*	action(Client& ref);
};
