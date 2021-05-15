#pragma once
#include "Client.hpp"
// #include "ClientStateStartLine.hpp"
// #include "ClientStateHead.hpp"
// #include "ClientStateBody.hpp"
// #include "ClientStateChunkedBody.hpp"
// #include "ClientStateDone.hpp"
// #include "ClientStateMethod.hpp"

class ClientState
{
	public:
		// static	ClientStateStartLine*	startline;
		// static	ClientStateHead*		head;
		// static	ClientStateBody*		body;
		// static	ClientStateChunkedBody*	chunkedbody;
		// static	ClientStateMethod*		method;
		// static	ClientStateDone*		done;
	public:
		ClientState();
		virtual ~ClientState();
		// virtual	void	done(Client& ref) = 0;
};