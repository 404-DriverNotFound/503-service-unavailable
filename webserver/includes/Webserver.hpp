#pragma once
#include "Bonus.hpp"
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
	/*==========================================================================
		Member Types
	==========================================================================*/
	public:
		typedef ConfigGlobal::port_container		port_container;
		typedef port_container::const_iterator		port_iterator;
		typedef map<string, ConfigServer>			server_container;
		typedef server_container::const_iterator	server_iterator;
		typedef map<string, ConfigLocation>			location_container;
		typedef location_container::const_iterator	location_iterator;
		typedef vector<Socket*>::iterator			socket_iterator;
		typedef list<Client*>::iterator				client_iterator;

	/*==========================================================================
		Static Member
	==========================================================================*/
	public:
		static ConfigGlobal						config;
		static vector<Socket*>					sockets;
		static FdSet							l_set;
		#ifdef __BONUS__
		static pthread_mutex_t					select_mutex;
		#endif

	/*==========================================================================
		Member
	==========================================================================*/
	private:
		FdSet									_o_set;
		FdSet									_r_set;
		FdSet									_w_set;
		FdSet									_e_set;
		list<Client*>							_clients;

	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		Webserver(const Webserver& ref);
		Webserver& operator=(const Webserver& ref);
	public:
		Webserver();
		~Webserver();
		/*------------------------
		getter
		------------------------*/

		/*------------------------
		setter
		------------------------*/


	/*==========================================================================
		Methods
	==========================================================================*/
	public:
		void			start_server();
	private:
		void			check_new_connection();
		void			manage_clients();

	/*==========================================================================
		Static Member Manager
	==========================================================================*/
	public:
		static void		init_static_members(int argc, char** argv, char** env);
		static void		destroy_static_members();
	private:
		static void		init_server_sockets(const ConfigGlobal::port_container ports);

	/*==========================================================================
		Exceptions
	==========================================================================*/
	private:
		class SelectFailed: public std::exception
		{	public: const char* what() const throw();	};

		class ConfigFileNoPort: public std::exception
		{	public: const char* what() const throw();	};
};