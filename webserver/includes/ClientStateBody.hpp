#pragma once

#include "ClientState.hpp"

class ClientStateBody : public ClientState
{
	private:
	public:
		ClientStateBody();
		~ClientStateBody();
		void	done(Client& ref);
};