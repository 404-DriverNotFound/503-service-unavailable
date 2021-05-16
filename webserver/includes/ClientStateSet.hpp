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
		void			check_auth(Client& ref);
		void			set_path(Client& ref);
		void			set_method(Client& ref);
};
