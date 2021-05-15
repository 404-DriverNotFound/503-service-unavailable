#pragma once
#include "ClientState.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "Path.hpp"
#include "Socket.hpp"
#include "Time.hpp"

class Client
{
	private:
		Socket						_socket;
		Time						_birth;
		map<string, ConfigServer>&	_servers;	// 참조할 서버 목록(포트에 종속)
		Path						_path;
		ConfigServer*				_server;
		ConfigLocation*				_location;
		ClientState*				_state;
		HttpReq						_req;
		HttpRes						_res;
	public:
		Client(int accept_fd, map<string, ConfigServer>& ref);
		~Client();
		void	routine();
		/*----------------------- 
		getter
		-----------------------*/
		HttpReq&	getReq(void);
};

// enum	e_client_status
// {
// 	client_start_line,
// 	client_head,
// 	client_body,
// 	client_set,
// 	client_method,
// 	// client_err,
// 	client_send,
// 	client_done,
// };

// class ClientStateSet : public ClientState
// {
// 	ClientState&		handle()
// 	{
// 		if (상태 끝)
// 		{
// 			if (메서드 == Get)
// 				// client->state = this->MethodGet;
// 				return this->method_get;
// 			else (method == Put)
// 				return this->method_put;
// 		}
// 	}
// };

// class ClientStateMethod : public ClientState
// {
	
// };

// class ClientStateMethodGet : public ClientStateMethod
// {
	
// };

// class ClientStateMethodHead : public ClientStateMethod
// {
	
// };

// class ClientStateMethodPut : public ClientStateMethod
// {
	
// };

// class ClientStateMethodPost : public ClientStateMethod
// {
	
// };

// class ClientStateMethodDelete : public ClientStateMethod
// {
	
// };

// class ClientStateMethodOption : public ClientStateMethod
// {
	
// };

// class ClientStateLoadHeader : public ClientState
// {
	
// };

// class ClientStateLoadBody : public ClientState
// {
	
// };

// class ClientStateSend : public ClientState
// {
	
// };

// class ClientStateDone : public ClientState
// {
	
// };