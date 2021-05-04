#pragma once
// #include "ConfigWebserver.hpp"
// #include "Path.hpp"
#include "FdSet.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Http.hpp"
#include "Buffer.hpp"
#include "Time.hpp"
#include "Cgi.hpp"
#include <sys/select.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <list>
/*#####################################
Webserver
######################################*/

using std::cout;
using std::endl;

using std::set;
using std::map;
using std::vector;
using std::deque;
using std::list;
using std::string;
using std::make_pair;

struct Webserver
{
	/*--------------------------------------------------------------------------
	typedef
	--------------------------------------------------------------------------*/
	typedef map<uint16_t, map<string, Server> >::iterator	server_iterator;
	typedef vector<Socket*>::iterator						socket_iterator;
	typedef list<Client*>::iterator							client_iterator;
	
	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	size_t								max_connection;
	Time								select_timeout;
	FdSet								o_set;
	FdSet								r_set;
	FdSet								w_set;
	FdSet								e_set;
	vector<Socket*>						sockets;
	map<uint16_t, map<string, Server> >	servers;
	list<Client*>						clients;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	private:
					Webserver(const Webserver& x);
	Webserver&		operator=(const Webserver& x);

	public:
					Webserver(int argc, char** argv, char** env);
					~Webserver();
	void			start_server();				// 서버 시작
	void			check_new_connection();
	void			manage_clients();
	void			set_status_code();
	void			set_path_cgi_bin(char** env);
	void			set_map_method();
	void			create_sockets();
	void			set_http_headers();

	/*----------------------
	Private Method
	-----------------------*/
	private:
	void			config_parser(deque<string>&, const char*);
	void			create_server(deque<string>&);
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
