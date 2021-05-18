#pragma once

#include "ClientState.hpp"

class ClientStateChunkedBodyCRLF : public ClientState
{
	private:
	public:
		ClientStateChunkedBodyCRLF();
		~ClientStateChunkedBodyCRLF();
		ClientState*	action(Client& ref);
};
