#pragma once

#include "State.hpp"

class StateMethod : public State
{
	private:
	public:
		StateMethod();
		~StateMethod();
		State*	action(Client& ref);


		State*		case_cgi(Client& ref);
		State*		case_dir(Client& ref);
		State*		case_file(Client& ref);
		State*		case_not_exist(Client& ref);

		State*		method_head(Client& ref);
		State*		method_get(Client& ref);
		State*		method_put(Client& ref);
		State*		method_delete(Client& ref);
};
