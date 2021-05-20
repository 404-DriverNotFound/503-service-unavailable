#pragma once

#include "State.hpp"

class StateBody : public State
{
	private:
	public:
		StateBody();
		~StateBody();
		State*	action(Client& ref);
};
