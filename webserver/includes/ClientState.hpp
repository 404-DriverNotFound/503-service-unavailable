#pragma once

#include "Client.hpp"
#include <iostream>
// #include "ClientStateStartLine.hpp"
// #include "ClientStateHead.hpp"
// #include "ClientStateBody.hpp"
// #include "ClientStateChunkedBody.hpp"
// #include "ClientStateDone.hpp"
// #include "ClientStateMethod.hpp"

class ClientStateStartLine;
class ClientStateHead;
class ClientStateBody;
class ClientStateChunkedBody;
class ClientStateMethod;
class ClientStateDone;

class ClientState
{
	public:
		static	ClientStateStartLine*		startline;
		static	ClientStateHead*			head;
		static	ClientStateBody*		body;
		static	ClientStateChunkedBody*	chunkedbody;
		static	ClientStateMethod*		method;
		static	ClientStateDone*		done;
	public:
		ClientState();
		virtual ~ClientState();
		virtual ClientState*	action(Client& ref) = 0;
};
