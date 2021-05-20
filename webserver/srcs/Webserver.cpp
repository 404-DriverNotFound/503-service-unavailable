#include "../includes/Webserver.hpp"
/*==============================================================================
	Static Member
==============================================================================*/

ConfigGlobal		Webserver::config;
vector<Socket*>		Webserver::sockets;
FdSet				Webserver::l_set;
#if __BONUS__ == 1
pthread_mutex_t		Webserver::select_mutex;
int					Webserver::prev_worker = 0;
#endif

/*==============================================================================
Constructor & Destructor
==============================================================================*/

Webserver::Webserver()
{
	_o_set = l_set;
}

//------------------------------------------------------------------------------

Webserver::~Webserver()
{
	while (!_clients.empty())
	{
		delete _clients.front();
		_clients.pop_front();
	}
}


const string&		Webserver::get_status_code(int code)
{
	return config.get_status_code(code);
}


/*==============================================================================
	Static Member Manager
==============================================================================*/

void		Webserver::init_static_members(int argc, char** argv, char** env)
{
	config = ConfigGlobal(argc, argv, env);
	init_server_sockets(config.get_ports());
	listen_server_sockets();
	#if __BONUS__ == 1
	pthread_mutex_init(&select_mutex, 0);
	File::mutex_init_tempname();
	string val = "0";
	config.set_select_timeout(val);
	#endif
}

//------------------------------------------------------------------------------

void		Webserver::destroy_static_members()
{
}

//------------------------------------------------------------------------------

void		Webserver::init_server_sockets(const ConfigGlobal::port_container ports)
{
	port_iterator it = ports.begin();
	port_iterator end = ports.end();

	sockets.reserve(ports.size());
	while (it != end)
	{
		sockets.push_back(new Socket(it->first, INADDR_ANY));
		l_set.set(sockets.back()->fd);
		++it;
	}
}

void		Webserver::listen_server_sockets()
{
	socket_iterator	it = sockets.begin();
	socket_iterator	end = sockets.end();

	while (it != end)
	{
		(*it)->listen(config.get_max_connection());
		++it;
	}
}

/*==============================================================================
	Methods
==============================================================================*/

void			Webserver::start_server()
{


	while (42)
	{
		// #if __BONUS__ == 1
		// pthread_mutex_lock(&select_mutex);
		// cout << "####################################################################\n";
		// prev_worker = worker_serial;

		// cout << "Worker " << worker_serial << endl;
		// #else
		// #endif


		cout << "####################################################################\n";
		if (select_routine() == 0)	// timeout
		{
			continue;
		}
		check_new_connection();
		manage_clients();
	}
}

//------------------------------------------------------------------------------

int				Webserver::select_routine()
{
	Time	select_timeout;
	_r_set = _o_set;
	_w_set = _o_set;
	_e_set = _o_set;
	select_timeout = config.get_select_timeout();
	int result = select(config.get_max_connection(), &_r_set.bits, &_w_set.bits, &_e_set.bits, (&select_timeout));
	if (result < 0)
	{
		perror("func: start server : ");
		throw SelectFailed();
	}
	return result;
}

void			Webserver::check_new_connection()
{
	socket_iterator		it = sockets.begin();
	socket_iterator		end = sockets.end();

	while (it != end)
	{
		if (_r_set.is_set((*it)->fd))
		{
			_clients.push_back(new Client((*it)->fd, config.get_server(htons((*it)->s_addr.sin_port)), _r_set, _w_set));
			cout << "new connection: " << (*it)->fd << " -> " << _clients.back()->get_socket().get_fd() << endl;
			_o_set.set(_clients.back()->get_socket().get_fd());
			// _r_set.clr((*it)->fd);
		}
		if (_e_set.is_set((*it)->fd))
		{
			cout << "Selet Error!" << endl;
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		++it;
	}
}

//------------------------------------------------------------------------------

void			Webserver::manage_clients()
{
	#ifdef DBG
	#endif

	char	temp[1];

	for (client_iterator it = _clients.begin() ; it != _clients.end() ; ++it)
	{
		if (_e_set.is_set((*it)->get_socket().fd))
		{
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		cout << "======================"<< endl;
		cout << "  socket: " << (*it)->get_socket().fd << endl;
		cout << "----------------------"<< endl;

		(*it)->routine();

		if ((*it)->get_state() == NULL)
		{
			cout << "delete socket: " << (*it)->get_socket().fd << endl;
			_o_set.clr((*it)->get_socket().get_fd());
			delete *it;
			it = _clients.erase(it);
		}
	}
}

/*==============================================================================
	Exceptions
==============================================================================*/

const char*	Webserver::SelectFailed::what() const throw()
{
	return ("Select Failed");
}
