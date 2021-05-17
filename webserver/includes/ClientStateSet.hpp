#pragma once

#include "ClientState.hpp"

class ClientStateSet : public ClientState
{
	private:
	public:
		ClientStateSet();
		~ClientStateSet();
		ClientState*	action(Client& ref);
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
