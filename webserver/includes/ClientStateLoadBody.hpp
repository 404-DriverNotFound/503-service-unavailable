#pragma once

#include "ClientState.hpp"

class ClientStateLoadBody : public ClientState
{
	private:
	public:
		ClientStateLoadBody();
		~ClientStateLoadBody();
		ClientState*	action(Client& ref);
};