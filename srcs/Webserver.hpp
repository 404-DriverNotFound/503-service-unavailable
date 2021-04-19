#pragma once
#include "ConfigWebserver.hpp"
#include "Path.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include <sys/select.h>
#include <vector>

struct Webserver
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
		void	config_parser(std::deque<std::string>&);
		void	server_create(std::deque<std::string>&);

	void			check_new_connection();
	void			manage_clients();

	public:	
	// exception
	class SelectFailed : public std::exception {
		public: virtual const char*	what() const throw();	};
	class InvalidServerBlock: public std::exception {
		public: virtual const char *what() const throw();	};
};

std::ostream&	operator<<(std::ostream&, WebServ&);