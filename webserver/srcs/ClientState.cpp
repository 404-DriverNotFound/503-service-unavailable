#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateHead.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateMethod.hpp"
#include "../includes/ClientStateDone.hpp"

/*-----------------
static
-----------------*/
ClientStateStartLine*	ClientState::startline		=	new ClientStateStartLine();
ClientStateHead*		ClientState::head			=	new	ClientStateHead();
// ClientStateBody*		ClientState::body			=	new	ClientStateBody();
// ClientStateChunkedBody*	ClientState::chunkedbody	=	new ClientStateChunkedBody();
// ClientStateMethod*		ClientState::method			=	new	ClientStateMethod();
// ClientStateDone*		ClientState::done			=	new	ClientStateDone();

ClientState::ClientState() {
}

ClientState::~ClientState() {
}

// int		main() {
// 	ClientState		state;
// }
