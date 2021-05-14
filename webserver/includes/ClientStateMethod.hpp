#pragma once

#include "ClientState.hpp"

class ClientStateMethod : public ClientState
{
	private:
	public:
		ClientStateMethod();
		~ClientStateMethod();
		void	done(Client& ref);
};