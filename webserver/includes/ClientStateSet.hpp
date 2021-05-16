#pragma once

#include "ClientState.hpp"

class ClientStateSet : public ClientState
{
	private:
	public:
		ClientStateSet();
		~ClientStateSet();
		ClientState*	action(Client& ref);
};
