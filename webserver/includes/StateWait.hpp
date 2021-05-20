#pragma once

#include "State.hpp"

class StateWait : public State
{
	private:
	public:
		StateWait();
		~StateWait();
		State*	action(Client& ref);
};