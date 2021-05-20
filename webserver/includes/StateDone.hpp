#pragma once

#include "State.hpp"

class StateDone : public State
{
	private:
	public:
		StateDone();
		~StateDone();
		State*	action(Client& ref);
};
