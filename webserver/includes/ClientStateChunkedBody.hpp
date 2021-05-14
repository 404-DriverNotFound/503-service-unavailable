#pragma once

#include "ClientState.hpp"

class ClientStateChunkedBody : public ClientState
{
	private:
	public:
		ClientStateChunkedBody();
		~ClientStateChunkedBody();
		void	done(Client& ref);
};