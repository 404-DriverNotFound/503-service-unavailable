#include "../includes/ClassModule.hpp"
#include "../includes/HttpReq.hpp"

Client::Client(int accept_fd, map<string, ConfigServer>& ref)
: _socket(accept_fd), _servers(ref)
{
}

Client::~Client()
{
	if (_method)
		delete _method;
}

void	Client::routine()
{
	switch(_status)
	{
		case client_start_line:
			_req.set_start_line();
			if (_status == client_start_line)
				break;
		case client_head:
			if (_status == client_head)
				break;
		//TODO recv를 위한 클래스를 구현 recv_body, recv_chunked_body	
		case client_body:
			if (_status == client_body)
				break;
		//TODO set_client();
		case client_method:
			if (_status == client_method)
				break;
		case client_done:
			if (_status == client_done)
		break;
	}
}