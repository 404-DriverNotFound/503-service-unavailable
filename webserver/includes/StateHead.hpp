#pragma once

#include "State.hpp"

class StateHead : public State
{
	private:
	public:
		StateHead();
		~StateHead();
		State*	action(Client& ref);
};
