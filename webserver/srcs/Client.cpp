#include "../includes/Webserver.hpp"
#include "../includes/Client.hpp"
#include "../includes/ClientState.hpp"
#include "../includes/ClientStateStartLine.hpp"
#include "../includes/ClientStateDone.hpp"

Client::Client(int accept_fd, const server_container& ref, FdSet& r_set, FdSet& w_set)
: _socket(accept_fd),
  _servers(ref),
  _r_set(r_set),
  _w_set(w_set),
  _state(ClientState::startline),
  _req(_socket.fd),
  _res(_socket.fd)
{
}

Client::~Client()
{
}

void	Client::routine()
{
	if (is_expired())
	{
		_state = 0;
		return;
	}
	recv_socket(_state->len);
	if(_state)
		_state = _state->action(*this);
	send_socket(_state->len);
}

/*=======================
getter
=======================*/
Socket&				Client::get_socket()
{
	return	_socket;
}

Time&				Client::get_time()
{
	return	_birth;
}

const Client::server_container&	Client::get_servers() const
{
	return	_servers;
}

const ConfigServer&		Client::get_server()
{
	return	*_server;
}

const ConfigLocation&	Client::get_location()
{
	return	*_location;
}

HttpReq&			Client::get_httpreq()
{
	return	_req;
}

HttpRes&			Client::get_httpres()
{
	return	_res;
}

ClientState*		Client::get_clientstate()
{
	return	_state;
}

bool				Client::get_next_line()
{
	return _req.get_next_line();
}

bool				Client::set_chunked_length()
{
	return _req.set_chunked_length();
}

/*=======================
setter
=======================*/
void				Client::set_server(const ConfigServer& svrp)
{
	_server = &svrp;
}

void				Client::set_location(const ConfigLocation& locp)
{
	_location = &locp;
}

void				Client::recv_socket(size_t len)
{
	if (_r_set.is_set(_socket.fd) == false)
		return ;
	std::cout << __func__ << std::endl;
	size_t test;
	if((test = _req.get_stream().fill(len)) == 0)
	{
		_state = NULL;
	}
	std::cout << "--------------" << std::endl;
	std::cout << "recv_byte: " << test << "state_len: " << len << std::endl;
	_req.get_stream().print();
}

void				Client::send_socket(size_t len)
{
	std::cout << __func__ << std::endl;
	size_t	test;
	test = _res.get_stream().pass();
	_res.get_stream().print();
	std::cout << "--------------" << std::endl;
	std::cout << "send_byte: " << test << std::endl;
}

bool				Client::read_chunked()
{
	return	_req.stream_to_body();
}

bool				Client::read_crlf()
{
	return	_req.read_crlf();
}

void				Client::update_birth()
{
	_birth.set_current();
}

bool				Client::is_expired()
{
	if ((Time() - _birth).get_time_usec() > Webserver::config.get_timeout().get_time_usec())
		return true;
	return false;
}