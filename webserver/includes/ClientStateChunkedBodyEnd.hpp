#pragma once

#include "ClientState.hpp"

class ClientStateChunkedBodyEnd : public ClientState
{
	private:
	public:
		ClientStateChunkedBodyEnd();
		~ClientStateChunkedBodyEnd();
		ClientState*	action(Client& ref);
};
