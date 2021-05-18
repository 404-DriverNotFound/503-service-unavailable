#pragma once

#include "Client.hpp"

class ClientStateStartLine;
class ClientStateHead;
class ClientStateSet;
class ClientStateBody;
class ClientStateChunkedBodyLen;
class ClientStateChunkedBody;
class ClientStateChunkedBodyCRLF;
class ClientStateChunkedBodyEnd;
class ClientStateMethod;
class ClientStateWait;
class ClientStateDone;

class ClientState
{
	public:
		static	ClientStateStartLine*		startline;
		static	ClientStateHead*			head;
		static	ClientStateSet*				bind;
		static	ClientStateBody*			body;
		static	ClientStateChunkedBodyLen*	chunkedbody_len;
		static	ClientStateChunkedBody*		chunkedbody;
		static	ClientStateChunkedBodyCRLF*	chunkedbody_crlf;
		static	ClientStateChunkedBodyEnd*	chunkedbody_end;
		static	ClientStateMethod*			method;
		static	ClientStateWait*			waiting;
		static	ClientStateDone*			done;
		size_t	len;
	public:
		ClientState();
		virtual ~ClientState();
		virtual ClientState*	action(Client& ref) = 0;
};
