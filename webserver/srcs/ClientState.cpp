#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateHead.hpp"
#include "../includes/ClientStateSet.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBodyLen.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateChunkedBodyCRLF.hpp"
#include "../includes/ClientStateChunkedBodyEnd.hpp"
#include "../includes/ClientStateMethod.hpp"
#include "../includes/ClientStateWait.hpp"
#include "../includes/ClientStateDone.hpp"

/*===================
static
===================*/
ClientStateStartLine*		ClientState::startline			= new ClientStateStartLine();
ClientStateHead*			ClientState::head				= new ClientStateHead();
ClientStateSet*				ClientState::bind				= new ClientStateSet();
ClientStateBody*			ClientState::body				= new ClientStateBody();
ClientStateChunkedBodyLen*	ClientState::chunkedbody_len	= new ClientStateChunkedBodyLen();
ClientStateChunkedBody*		ClientState::chunkedbody		= new ClientStateChunkedBody();
ClientStateChunkedBodyCRLF*	ClientState::chunkedbody_crlf	= new ClientStateChunkedBodyCRLF();
ClientStateChunkedBodyEnd*	ClientState::chunkedbody_end	= new ClientStateChunkedBodyEnd();
ClientStateMethod*			ClientState::method				= new ClientStateMethod();
ClientStateWait*			ClientState::waiting			= new ClientStateWait();
ClientStateDone*			ClientState::done				= new ClientStateDone();

/*===================
constructor
===================*/
ClientState::ClientState()
{
}

/*===================
destructor
===================*/
ClientState::~ClientState()
{
}