#pragma once
// #include "ConfigWebserver.hpp"
// #include "Path.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Http.hpp"
#include "Buffer.hpp"
#include "Time.hpp"
#include <sys/select.h>
#include <map>
#include <vector>
#include <list>
/*#####################################
Webserver
######################################*/
struct Webserver
{
	/*-----------------------
	typedef
	-----------------------*/
	typedef std::map<std::string, Server>::iterator	server_iterator;
	typedef std::vector<Socket*>::iterator			socket_iterator;
	typedef std::list<Client>::iterator				client_iterator;
	
	/*------------------------
	Member
	-------------------------*/
	size_t					max_connection;
	Time					select_timeout;
	FdSet					to_be_checked;
	FdSet					to_be_checked_read;
	FdSet					to_be_checked_write;
	FdSet					to_be_checked_exception;
	std::set<uint16_t>		ports;
	std::vector<Socket*>	sockets;
	std::map<std::string, Server>		servers;
	std::list<Client>		clients;

	Webserver(int argc, char** argv, char** env);
	Webserver(const std::string& path_config);
	~Webserver();
	/*----------------------
	Method
	-----------------------*/
	void			start_server();				// 서버 시작
	void			check_new_connection();
	void			manage_clients();
	void			set_status_code();
	void			set_path_cgi_bin(char** env);
	void			set_map_method();
	void			create_sockets();

	/*----------------------
	Private Method
	-----------------------*/
	private:
	void			config_parser(std::deque<std::string>&, const char*);
	void			server_create(std::deque<std::string>&);
	void			put_port_numbers();

	/*------------------------------
	Exception
	------------------------------*/
	public:	
	class SelectFailed : public std::exception {
		public: virtual const char*	what() const throw();	};
	class InvalidServerBlock: public std::exception {
		public: virtual const char *what() const throw();	};
};

std::ostream&	operator<<(std::ostream&, Webserver&);
