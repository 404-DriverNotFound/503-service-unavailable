#pragma once

#include "Client.hpp"

class ClientStateStartLine;
class ClientStateHead;
class ClientStateSet;
class ClientStateBody;
class ClientStateChunkedBody;
class ClientStateMethod;
class ClientStateDone;

class ClientState
{
	public:
		static	ClientStateStartLine*	startline;
		static	ClientStateHead*		head;
		static	ClientStateSet*			set;
		static	ClientStateBody*		body;
		static	ClientStateChunkedBody*	chunkedbody;
		static	ClientStateMethod*		method;
		static	ClientStateDone*		done;
	public:
		ClientState();
		virtual ~ClientState();
		virtual ClientState*	action(Client& ref) = 0;
};
