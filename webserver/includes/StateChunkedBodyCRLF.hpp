#pragma once

#include "State.hpp"

class StateChunkedBodyCRLF : public State
{
	private:
	public:
		StateChunkedBodyCRLF();
		~StateChunkedBodyCRLF();
		State*	action(Client& ref);
};
