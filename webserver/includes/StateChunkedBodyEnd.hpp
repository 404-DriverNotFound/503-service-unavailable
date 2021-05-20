#pragma once

#include "State.hpp"

class StateChunkedBodyEnd : public State
{
	private:
	public:
		StateChunkedBodyEnd();
		~StateChunkedBodyEnd();
		State*	action(Client& ref);
};
