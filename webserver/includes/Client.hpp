#pragma once
#include "Socket.hpp"
#include "Stream.hpp"
#include "Time.hpp"
#include "Path.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "FdSet.hpp"

class ClientState;
/*##############################################################################
	Client
##############################################################################*/
class Client
{
	/*==========================================================================
		Member Type
	==========================================================================*/
	public:
		typedef	map<string, ConfigServer>	ServerMap;
	/*==========================================================================
		Member
	==========================================================================*/
	private:
		Socket						_socket;
		ServerMap&					_servers;	// 참조할 서버 목록(포트에 종속)
		FdSet&						_r_set;
		FdSet&						_w_set;
		Time						_birth;
		const ConfigServer*			_server;
		const ConfigLocation*		_location;
		ClientState*				_state;
		HttpReq						_req;
		HttpRes						_res;
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		Client();
		Client(const Client&);
		Client& operator=(const Client&);
	public:
		Client(int accept_fd, ServerMap& ref, FdSet& r_set, FdSet& w_set);
		~Client();
	/*==========================================================================
		Method
	==========================================================================*/	
		void						routine();
		void						recv_socket(size_t len);
		void						send_socket(size_t len);
		bool						get_next_line();
	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		Socket&						get_socket();
		Time&						get_time();
		ServerMap&					get_servers();
		const ConfigServer&			get_server();
		const ConfigLocation&		get_location();
		HttpReq&					get_httpreq();
		HttpRes&					get_httpres();
		ClientState*				get_clientstate();
	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void						set_server(const ConfigServer& svrp);
		void						set_location(const ConfigLocation& locp);
		// chunked
		bool						set_chunked_length();
		bool						read_chunked();
		bool						read_crlf();
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
