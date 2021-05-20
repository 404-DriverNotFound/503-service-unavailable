#pragma once

#include "State.hpp"

class StateLoadBody : public State
{
	private:
	public:
		StateLoadBody();
		~StateLoadBody();
		State*	action(Client& ref);
};