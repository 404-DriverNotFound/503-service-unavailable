#pragma once
#include "Cgi.hpp"
#include "Socket.hpp"
#include "Stream.hpp"
#include "Time.hpp"
#include "Path.hpp"
#include "ConfigServer.hpp"
#include "ConfigLocation.hpp"
#include "HttpReq.hpp"
#include "HttpRes.hpp"
#include "FdSet.hpp"

class State;
/*##############################################################################
	Client
##############################################################################*/
class Client
{
	/*==========================================================================
		Member Type
	==========================================================================*/
	public:
		typedef	map<string, ConfigServer>	server_container;
	/*==========================================================================
		Member
	==========================================================================*/
	private:
		Socket						_socket;
		const server_container&		_servers;	// 참조할 서버 목록(포트에 종속)
		FdSet&						_r_set;
		FdSet&						_w_set;
		Time						_birth;
		const ConfigServer*			_server;
		const ConfigLocation*		_location;
		State*						_state;
		HttpReq						_req;
		HttpRes						_res;
	public:
		Cgi*						_cgi;
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		Client();
		Client(const Client&);
		Client& operator=(const Client&);
	public:
		Client(int accept_fd, const server_container& ref, FdSet& r_set, FdSet& w_set);
		~Client();
	/*==========================================================================
		Method
	==========================================================================*/
		int							client_action();
		void						routine();
		void						recv_socket(size_t len);
		void						send_socket();
		void						update_birth();
		bool						get_next_line();
		bool						is_expired();
		void						make_meta_variable();
		void						run_cgi();
		void						del_cgi();
		void						clear();
		void						print_meta_variable();
	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		Socket&						get_socket();
		Time&						get_time();
		const server_container&		get_servers() const;
		const ConfigServer&			get_server();
		const ConfigLocation&		get_location();
		HttpReq&					get_httpreq();
		HttpRes&					get_httpres();
		State*						get_state();
		const string&				get_method();
	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void						set_server(const ConfigServer& svrp);
		void						set_location(const ConfigLocation& locp);
		bool						set_chunked_length();
		bool						read_chunked();
		bool						read_crlf();
		void						set_cgi();
		bool						check_cgi_exit();
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

// class tStateSet : public tState
// {
// 	tState&		handle()
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

// class tStateMethod : public tState
// {
	
// };

// class tStateMethodGet : public tStateMethod
// {
	
// };

// class tStateMethodHead : public tStateMethod
// {
	
// };

// class tStateMethodPut : public tStateMethod
// {
	
// };

// class tStateMethodPost : public tStateMethod
// {
	
// };

// class tStateMethodDelete : public tStateMethod
// {
	
// };

// class tStateMethodOption : public tStateMethod
// {
	
// };

// class tStateLoadHeader : public tState
// {
	
// };

// class tStateLoadBody : public tState
// {
	
// };

// class tStateSend : public tState
// {
	
// };

// class tStateDone : public tState
// {
	
// };
