#pragma once
#include "Socket.hpp"
#include "Stream.hpp"
#include "Time.hpp"
#include "Path.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"

class ClientState;

class Client
{
	typedef	map<string, ConfigServer>	ServerMap;
	private:
		Socket						_socket;
		Time						_birth;
		ServerMap&					_servers;	// 참조할 서버 목록(포트에 종속)
		const ConfigServer*			_server;
		const ConfigLocation*		_location;
		HttpReq						_req;
		HttpRes						_res;
		ClientState*				_state;
	public:
		Client(int accept_fd, ServerMap& ref);
		~Client();
		void	routine();
		/*=======================
		getter
		=======================*/
		Socket&					get_socket();
		Time&					get_time();
		ServerMap&				get_servers();
		const ConfigServer&		get_server();
		const ConfigLocation&	get_location();
		HttpReq&				get_httpreq();
		HttpRes&				get_httpres();
		/*=======================
		setter
		=======================*/
		void				set_server(const ConfigServer& svrp);
		void				set_location(const ConfigLocation& locp);
		/*=======================
		method
		=======================*/
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
