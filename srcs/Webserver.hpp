#pragma once
// #include "ConfigWebserver.hpp"
// #include "Path.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Http.hpp"
#include "Buffer.hpp"
#include <sys/select.h>
#include <vector>
/*#####################################
Webserver
######################################*/
struct Webserver
{
	/*-----------------------
	typedef
	-----------------------*/
	typedef std::vector<Server>::iterator	server_iterator;
	typedef std::vector<Client>::iterator	client_iterator;
	
	/*------------------------
	Member
	-------------------------*/
	size_t					max_connection;

	FdSet					to_be_checked;
	FdSet					to_be_checked_read;
	FdSet					to_be_checked_write;
	FdSet					to_be_checked_exception;
	std::vector<Server>		servers;
	std::vector<Client>		clients;

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

	/*----------------------
	Private Method
	-----------------------*/
	private:
	void			config_parser(std::deque<std::string>&, const char*);
	void			server_create(std::deque<std::string>&);

	public:	
	/*------------------------------
	Exception
	------------------------------*/
	class SelectFailed : public std::exception {
		public: virtual const char*	what() const throw();	};
	class InvalidServerBlock: public std::exception {
		public: virtual const char *what() const throw();	};
};

std::ostream&	operator<<(std::ostream&, Webserver&);
