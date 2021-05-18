#include "../includes/Webserver.hpp"
/*==============================================================================
	Static Member
==============================================================================*/

ConfigGlobal		Webserver::config;
vector<Socket*>		Webserver::sockets;
FdSet				Webserver::l_set;
#ifdef __BONUS__
pthread_mutex_t		Webserver::select_mutex;
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

/*==============================================================================
	Static Member Manager
==============================================================================*/

void		Webserver::init_static_members(int argc, char** argv, char** env)
{
	config = ConfigGlobal(argc, argv, env);
	init_server_sockets(config.get_ports());
	pthread_mutex_init(&select_mutex, 0);
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

/*==============================================================================
	Methods
==============================================================================*/

void			Webserver::start_server()
{
	int		result;

	socket_iterator	it = sockets.begin();
	socket_iterator	end = sockets.end();
	Time			select_timeout;

	while (it != end)
	{
		(*it)->listen(config.get_max_connection());
		++it;
	}
	while (42)
	{
		#ifdef __BONUS__
		pthread_mutex_lock(&select_mutex);
		#endif

		_r_set = _o_set;
		_w_set = _o_set;
		_e_set = _o_set;
		select_timeout = config.get_select_timeout();
		cout << "select_timeout  " << select_timeout << endl;
		result = select(config.get_max_connection(), &_r_set.bits, &_w_set.bits, &_e_set.bits, (&select_timeout));
		if (result < 0)
		{
			perror("func: start server : ");
			throw SelectFailed();
		}
		if (result == 0)	// timeout
		{
			continue;
		}

		#ifdef __BONUS__
		pthread_mutex_unlock(&select_mutex);
		#endif

		check_new_connection();
		manage_clients();
	}
}

//------------------------------------------------------------------------------

void			Webserver::check_new_connection()
{
	socket_iterator		it = sockets.begin();
	socket_iterator		end = sockets.end();
	// cout << '\n' <<  __func__ << "\n======================================"<< endl;
	while (it != end)
	{
		if (_r_set.is_set((*it)->fd))
		{
			_clients.push_back(new Client((*it)->fd, config.get_server(htons((*it)->s_addr.sin_port)), _r_set, _w_set));
			_o_set.set(_clients.back()->get_socket().get_fd());
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
	// #ifdef DBG
	// cout << '\n' << __func__ << "\n======================================"<< endl;
	// #endif
	char	temp[1];

	for (client_iterator it = _clients.begin() ; it != _clients.end() ; ++it)
	{
		if (_e_set.is_set((*it)->get_socket().fd))
		{
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		(*it)->routine();
		if ((*it)->get_clientstate() == NULL)
		{
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
