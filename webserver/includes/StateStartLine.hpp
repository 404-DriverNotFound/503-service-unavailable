#pragma once

#include "State.hpp"

class StateStartLine : public State
{
	private:
	public:
		StateStartLine();
		~StateStartLine();
		State*	action(Client& ref);
};
