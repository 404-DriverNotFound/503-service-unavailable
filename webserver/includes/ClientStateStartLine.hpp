#pragma once

#include "ClientState.hpp"

class ClientStateStartLine : public ClientState
{
	private:
	public:
		ClientStateStartLine();
		~ClientStateStartLine();
		void	done(Client& ref);
};
