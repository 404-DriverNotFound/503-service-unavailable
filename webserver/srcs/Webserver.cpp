#include "../includes/Webserver.hpp"

ConfigGlobal					Webserver::config;
FdSet							Webserver::o_set;
FdSet							Webserver::r_set;
FdSet							Webserver::w_set;
FdSet							Webserver::e_set;
vector<Socket*>					Webserver::sockets;

Webserver::Webserver()
{
}

Webserver::~Webserver()
{
}

void		Webserver::init_static_members(int argc, char** argv, char** env)
{
	config = ConfigGlobal(argc, argv, env);
	init_server_socket(config.ports);
}

void		Webserver::init_server_socket(const ConfigGlobal::port_container ports)
{
	port_iterator it = config.ports.begin();
	port_iterator end = config.ports.end();

	sockets.reserve(config.ports.size());
	while (it != end)
	{
		sockets.push_back(new Socket(it->first, INADDR_ANY));
		o_set.set(sockets.back()->fd);
		++it;
	}	
}