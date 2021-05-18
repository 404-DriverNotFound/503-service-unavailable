#pragma once

#include "ClientState.hpp"

class ClientStateChunkedBodyLen : public ClientState
{
	private:
	public:
		ClientStateChunkedBodyLen();
		~ClientStateChunkedBodyLen();
		ClientState*	action(Client& ref);
};
