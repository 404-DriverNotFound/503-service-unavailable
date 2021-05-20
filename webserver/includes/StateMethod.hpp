#pragma once

#include "State.hpp"

class StateMethod : public State
{
	private:
	public:
		StateMethod();
		~StateMethod();
		State*	action(Client& ref);
};
