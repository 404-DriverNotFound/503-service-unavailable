#pragma once

#include "ClientState.hpp"

class ClientStateChunkedBody : public ClientState
{
	private:
	public:
		ClientStateChunkedBody();
		~ClientStateChunkedBody();
		ClientState*	action(Client& ref);
};
