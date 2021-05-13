#pragma once
#include "ClassModule.hpp"

class Client
{
	enum	e_client_status
	{
		client_start_line,
		client_head,
		client_body,
		client_set,
		client_method,
		// client_err,
		client_send,
		client_done,
	};
	private:
		Socket						_socket;
		Time						_birth;
		map<string, ConfigServer>&	_servers;	// 참조할 서버 목록(포트에 종속)
		Path						_path;
		ConfigServer*				_server;
		ConfigLocation*				_location;
		AMethod*					_method;
		e_client_status				_status;
		HttpReq						_req;
		HttpRes						_res;
		ClientState&				state;
	public:
		Client(int accept_fd, map<string, ConfigServer>& ref);
		~Client();
		void	routine();
};

class ClientState
{
	static	ClientStateStartLine	startline;
	static	ClientStateHead			header;
	static	ClientStateBody			body;

	public:
		ClientState& handle(Client& ref);
};

class ClientStateStartLine : public ClientState
{

};

class ClientStateHead : public ClientState
{

};

class ClientStateBody : public ClientState
{

}; 

class ClientStateBodyChunked : public ClientState
{

}; 

class ClientStateSet : public ClientState
{
	ClientState&		handle()
	{
		if (상태 끝)
		{
			if (메서드 == Get)
				// client->state = this->MethodGet;
				return this->method_get;
			else (method == Put)
				return this->method_put;
		}
	}
};

class ClientStateMethod : public ClientState
{
	
};

class ClientStateMethodGet : public ClientStateMethod
{
	
};

class ClientStateMethodHead : public ClientStateMethod
{
	
};

class ClientStateMethodPut : public ClientStateMethod
{
	
};

class ClientStateMethodPost : public ClientStateMethod
{
	
};

class ClientStateMethodDelete : public ClientStateMethod
{
	
};

class ClientStateMethodOption : public ClientStateMethod
{
	
};

class ClientStateLoadHeader : public ClientState
{
	
};

class ClientStateLoadBody : public ClientState
{
	
};

class ClientStateSend : public ClientState
{
	
};

class ClientStateDone : public ClientState
{
	
};





