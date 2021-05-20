#pragma once

#include "State.hpp"

class StateChunkedBodyLen : public State
{
	private:
	public:
		StateChunkedBodyLen();
		~StateChunkedBodyLen();
		State*	action(Client& ref);
};
