#pragma once

#include "State.hpp"

class StateSet : public State
{
	private:
	public:
		StateSet();
		~StateSet();
		State*	action(Client& ref);
		static State*	check_transfer_type(Client& ref);
		void			set_server(Client& ref);
		void			set_location(Client& ref);
		void			check_method(Client& ref);
		void			check_auth(Client& ref);
		/*============
		setter
		============*/
		void			set_file(Client& ref);
		void			set_method(Client& ref);
};
