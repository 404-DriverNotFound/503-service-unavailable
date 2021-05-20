#pragma once

#include "State.hpp"

class StateChunkedBody : public State
{
	private:
	public:
		StateChunkedBody();
		~StateChunkedBody();
		State*	action(Client& ref);
};
