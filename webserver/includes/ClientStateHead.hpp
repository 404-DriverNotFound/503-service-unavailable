#pragma once

#include "ClientState.hpp"

class ClientStateHead : public ClientState
{
	private:
	public:
		ClientStateHead();
		~ClientStateHead();
		void	done(Client& ref);
};
