#include "../includes/Webserver.hpp"
#include "../includes/Client.hpp"
#include "../includes/State.hpp"
#include "../includes/StateSet.hpp"
#include "../includes/StateStartLine.hpp"
#include "../includes/StateDone.hpp"

Client::Client(int accept_fd, const server_container& ref, FdSet& r_set, FdSet& w_set)
: _socket(accept_fd),
  _servers(ref),
  _r_set(r_set),
  _w_set(w_set),
  _state(State::startline),
  _req(_socket.fd),
  _res(_socket.fd)
{
}

Client::~Client()
{
}

/*==============================================================================
Method
==============================================================================*/

void				Client::recv_socket(size_t len)
{
	cout << __func__ << endl;
	if (_r_set.is_set(_socket.fd) == false)
		return ;
	size_t test;
	if((test = _req.get_stream().fill(len)) == 0)
	{
		_state = NULL;
	}

	cout <<   "vvvvvvvvvvv recv vvvvvvvvvvvvvv" << endl;
	_req.get_stream().print();
	cout << "\n^^^^^^^^^^^ recv ^^^^^^^^^^^^^^" << endl;

	// cout << "recv_byte: " << test << endl << "state_len: " << len << endl;
}
//------------------------------------------------------------------------------
void				Client::send_socket(size_t len)
{
	cout << __func__ << endl;
	if (_w_set.is_set(_socket.fd) == false)
		return ;

	cout <<   "vvvvvvvvvvv send vvvvvvvvvvvvvv" << endl;
	_res.get_stream().print_line();
	cout << "\n^^^^^^^^^^^ send ^^^^^^^^^^^^^^" << endl;

	if (get_httpres().load_body())
	{
		_state = State::done;
	}
}
//------------------------------------------------------------------------------
void				Client::update_birth()
{
	_birth.set_current();
}
//------------------------------------------------------------------------------
bool				Client::is_expired()
{
	if ((Time() - _birth).get_time_usec() > Webserver::config.get_timeout().get_time_usec())
		return true;
	return false;
}
//------------------------------------------------------------------------------
void	Client::routine()
{
	usleep(500000);
	if (is_expired())
	{
		_state = 0;
		return;
	}
	recv_socket(_state->len);
	try
	{
		if (client_action())
			return;	
	}
	catch(int code)
	{
		cout << "mange error!\n";
		const string&	path = _req.get_path().get_path_translated();

		_res.set_status_code(code);
		if (_req.set_index_page(_location->get_error_page()) == false)	// error page not found
		{
			_res.set_file(File::o_create);
			::write(_res.get_file_fd(), "Error!", 6);
			lseek(_res.get_file_fd(), SEEK_SET, 0);
		}
		else
		{
			_res.set_file(path, File::o_read);
		}
		_state = StateSet::check_transfer_type(*this);
	}
	send_socket(_state->len);
}
//------------------------------------------------------------------------------
int					Client::client_action()
{
	if(_state)
	{
		State*	prev;
		do
		{
			prev = _state;
			_state = _state->action(*this);
		}
		while (_state && prev != _state);
		return false;
	}
	else
	{
		return true;	// end of client
	}
}
//------------------------------------------------------------------------------
/*==============================================================================
setter
==============================================================================*/
void				Client::set_server(const ConfigServer& svrp)
{
	_server = &svrp;
}
//------------------------------------------------------------------------------
void				Client::set_location(const ConfigLocation& locp)
{
	_location = &locp;
}
//------------------------------------------------------------------------------

bool				Client::read_chunked()
{
	return	_req.stream_to_body();
}
//------------------------------------------------------------------------------
bool				Client::read_crlf()
{
	return	_req.read_crlf();
}
//------------------------------------------------------------------------------

bool				Client::set_chunked_length()
{
	return _req.set_chunked_length();
}

/*==============================================================================
getter
==============================================================================*/

Socket&				Client::get_socket()
{
	return	_socket;
}
//------------------------------------------------------------------------------
Time&				Client::get_time()
{
	return	_birth;
}
//------------------------------------------------------------------------------
const Client::server_container&	Client::get_servers() const
{
	return	_servers;
}
//------------------------------------------------------------------------------
const ConfigServer&		Client::get_server()
{
	return	*_server;
}
//------------------------------------------------------------------------------
const ConfigLocation&	Client::get_location()
{
	return	*_location;
}
//------------------------------------------------------------------------------
HttpReq&			Client::get_httpreq()
{
	return	_req;
}
//------------------------------------------------------------------------------
HttpRes&			Client::get_httpres()
{
	return	_res;
}
//------------------------------------------------------------------------------
State*		Client::get_state()
{
	return	_state;
}
//------------------------------------------------------------------------------
bool				Client::get_next_line()
{
	return _req.get_next_line();
}
