#pragma once

#include "Client.hpp"

class StateStartLine;
class StateHead;
class StateSet;
class StateBody;
class StateChunkedBodyLen;
class StateChunkedBody;
class StateChunkedBodyCRLF;
class StateChunkedBodyEnd;
class StateMethod;
class StateLoadBody;
class StateWait;
class StateDone;

class State
{
	public:
		static	StateStartLine*		startline;
		static	StateHead*			head;
		static	StateSet*				bind;
		static	StateBody*			body;
		static	StateChunkedBodyLen*	chunkedbody_len;
		static	StateChunkedBody*		chunkedbody;
		static	StateChunkedBodyCRLF*	chunkedbody_crlf;
		static	StateChunkedBodyEnd*	chunkedbody_end;
		static	StateMethod*			method;
		static	StateLoadBody*		loadbody;
		static	StateWait*			waiting;
		static	StateDone*			done;
		size_t	len;
	public:
		State();
		virtual ~State();
		virtual State*	action(Client& ref) = 0;
};
