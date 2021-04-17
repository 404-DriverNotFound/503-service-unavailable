#pragma once
#include "ConfigWebserver.hpp"
#include "Path.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include <sys/select.h>
#include <vector>

class Webserver
{
	ConfigWebserver			config;
	FdSet					to_be_checked;
	FdSet					to_be_checked_read;
	FdSet					to_be_checked_write;
	FdSet					to_be_checked_exception;
	std::vector<Server>		servers;
	std::vector<Client>		clients;

	public:
	typedef std::vector<Server>::iterator	server_iterator;
	typedef std::vector<Client>::iterator	client_iterator;

					Webserver(const Path&); 	// config 파일의 경로를 받아서 초기화
	void			create_server(const std::vector<Config>& config_locations);
	void			start_server();				// 서버 시작

	private:
	void			parse_config(const Path& path_config);			// config 파일 해석
	void			check_new_connection();
	void			manage_clients();

	public:	
	// exception
	class SelectFailed : public std::exception {
		const char*	what() const throw();	};
};