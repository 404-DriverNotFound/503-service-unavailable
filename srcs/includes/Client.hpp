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
	public:
		Client(int accept_fd, map<string, ConfigServer>& ref);
		~Client();
		void	routine();
};