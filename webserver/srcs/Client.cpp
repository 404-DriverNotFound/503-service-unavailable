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
	if (_req.get_stream().size() < 1000)
		_req.get_stream().print();
	else
	{
		cout << "len: " << _req.get_stream().size() << endl;
	}
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
	if (_res.get_stream().size() < 1000)
		_res.get_stream().print_line();
	else
	{
		cout << "len: " << _res.get_stream().size() << endl;
	}
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
	// usleep(300000);
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
void			Client::run_cgi()
{
	_cgi->start_cgi();
}


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

void				Client::set_cgi()
{
	_cgi = new Cgi(_req.get_path()._path_translated, _req.get_path()._extension, _req.get_file_fd(), _res.get_file_fd());
}

void				Client::make_meta_variable()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif

	_cgi->meta_variables.reserve(17 + _req.get_headers().size());
	_cgi->meta_variables.assign(17, string());

	_cgi->meta_variables[0] = string("AUTH_TYPE="		).append(_location->get_auth_type());
	_cgi->meta_variables[1] = string("CONTENT_LENGTH="	).append(_req.get_header("CONTNET_LENGTH"));
	_cgi->meta_variables[2] = string("CONTENT_TYPE="		).append(_req.get_header("CONTNET_TYPE"));
	_cgi->meta_variables[3] = string("GATEWAY_INTERFACE=").append("CGI/1.1");
	_cgi->meta_variables[4] = string("PATH_INFO="		).append(_req.get_path().get_path_info());
	_cgi->meta_variables[5] = string("PATH_TRANSLATED="	).append(_req.get_path().get_path_translated());
	_cgi->meta_variables[6] = string("QUERY_STRING="		).append(_req.get_path()._query);
	_cgi->meta_variables[7] = string("REMOTE_ADDR="		).append("");
	_cgi->meta_variables[8] = string("REMOTE_IDENT="		).append(_req.get_header("AUTHORIZATION"));
	_cgi->meta_variables[9] = string("REMOTE_USER="		).append("");
	_cgi->meta_variables[10] = string("REQUEST_METHOD="	).append(get_method());
	_cgi->meta_variables[11] = string("REQUEST_URI="		).append(_req.get_path().get_path_info());
	_cgi->meta_variables[12] = string("SCRIPT_NAME="		).append(Webserver::get_cgi_bin(_req.get_path()._extension));
	_cgi->meta_variables[13] = string("SERVER_NAME="		).append(_server->get_name());
	_cgi->meta_variables[14] = string("SERVER_PORT="		).append(ft::itoa(_server->get_port()));
	_cgi->meta_variables[15] = string("SERVER_PROTOCOL="	).append("HTTP/1.1");
	_cgi->meta_variables[16] = string("SERVER_SOFTWARE="	).append("Webserver42/1.0.0");

	map<string, string>::iterator	it_header = _req.get_headers().begin();
	map<string, string>::iterator	it_header_end = _req.get_headers().end();

	while (it_header != it_header_end)
	{
		_cgi->meta_variables.push_back(string());
		_cgi->meta_variables.back().reserve(100);
		_cgi->meta_variables.back() += "HTTP_";
		_cgi->meta_variables.back() += it_header->first;
		_cgi->meta_variables.back() += "=";
		_cgi->meta_variables.back() += it_header->second;
		++it_header;
	}

	// for (vector<string>::iterator it = _cgi->meta_variables.begin(); it != _cgi->meta_variables.end() ; ++it)
	// {
	// 	cout << *it << endl;
	// }
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
//------------------------------------------------------------------------------
const string&		Client::get_method()
{
	return _req.get_method();
}

bool				Client::check_cgi_exit()
{
	return	_cgi->check_exit();
}