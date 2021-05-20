#include "../includes/State.hpp"
#include "../includes/StateCgi.hpp"
#include "../includes/StateStartLine.hpp"
#include "../includes/StateHead.hpp"
#include "../includes/StateSet.hpp"
#include "../includes/StateBody.hpp"
#include "../includes/StateChunkedBodyLen.hpp"
#include "../includes/StateChunkedBody.hpp"
#include "../includes/StateChunkedBodyCRLF.hpp"
#include "../includes/StateChunkedBodyEnd.hpp"
#include "../includes/StateMethod.hpp"
#include "../includes/StateLoadBody.hpp"
#include "../includes/StateWait.hpp"
#include "../includes/StateDone.hpp"

/*===================
static
===================*/
StateStartLine*			State::startline		= new StateStartLine();
StateHead*				State::head				= new StateHead();
StateSet*				State::bind				= new StateSet();
StateBody*				State::body				= new StateBody();
StateChunkedBodyLen*	State::chunkedbody_len	= new StateChunkedBodyLen();
StateChunkedBody*		State::chunkedbody		= new StateChunkedBody();
StateChunkedBodyCRLF*	State::chunkedbody_crlf	= new StateChunkedBodyCRLF();
StateChunkedBodyEnd*	State::chunkedbody_end	= new StateChunkedBodyEnd();
StateMethod*			State::method			= new StateMethod();
StateLoadBody*			State::loadbody			= new StateLoadBody();
StateWait*				State::waiting			= new StateWait();
StateDone*				State::done				= new StateDone();
StateCgi*				State::cgi				= new StateCgi();

/*===================
constructor
===================*/
State::State()
{
}

/*===================
destructor
===================*/
State::~State()
{
}