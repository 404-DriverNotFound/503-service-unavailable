#pragma once
#include "ConfigGlobal.hpp"
#include "FdSet.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include <vector>
#include <list>

using std::list;
using std::vector;
using std::string;

class Webserver
{
	public:
		typedef ConfigGlobal::port_container	port_container;
		typedef port_container::iterator		port_iterator;
		typedef map<string, ConfigServer>		server_container;
		typedef server_container::iterator		server_iterator;
		typedef map<string, ConfigLocation>		location_container;
		typedef location_container::iterator	location_iterator;
		typedef vector<Socket*>::iterator		socket_iterator;
		typedef list<Client*>::iterator			client_iterator;

	public:
		static ConfigGlobal						config;
		static FdSet							o_set;
		static FdSet							r_set;
		static FdSet							w_set;
		static FdSet							e_set;
		static vector<Socket*>					sockets;

	private:
		list<Client*>							_clients;

	private:
		Webserver(const Webserver& ref);
		Webserver& operator=(const Webserver& ref);

	public:
		/*------------------------
		Constructor & Destructor
		------------------------*/
		Webserver();
		~Webserver();
		/*------------------------
		getter
		------------------------*/

		/*------------------------
		setter
		------------------------*/


		/*------------------------
		Method
		------------------------*/
		void			start_server();
		static void		init_static_members(int argc, char** argv, char** env);
		static void		init_server_socket(const ConfigGlobal::port_container ports);
		
};