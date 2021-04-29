#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "../includes/Webserver.hpp"
#define DBG

/*#####################################
Webserver
######################################*/

Webserver::Webserver(int argc, char** argv, char** env)
: max_connection(100), select_timeout(5000000)
{
	if (argc > 2)
	{
		std::cout << "Usage: " << argv[0] << " [configfile]" << std::endl;
		return ;
	}
	set_path_cgi_bin(env);
	HttpRes::init_status_code();
	Http::init_map_headers();
	Http::init_map_methods();
	Method::init_method_num();
	Method::init_method_flags();
	Method::init_method_strings();
	deque<std::string>	token(1);
	

	config_parser(token, argv[1]);
	server_create(token);
	create_sockets();

	// cout << *this << endl;
	try
	{
		start_server();
	}
	catch(int a)
	{
		std::cerr << "code : " << a << '\n';
	}
	
	cout << "end" << endl;
}

//------------------------------------------------------------------------------

Webserver::~Webserver()
{}

//------------------------------------------------------------------------------

void	Webserver::config_parser(deque<string>& token, const char* config_path)
{
	char						buf[30];
	int							fd, n, idx, vdx = 0;

	if (!config_path)
	{
		config_path = "./server_config";
	}
	if ((fd = open(config_path, O_RDONLY)) < 0)
	{
		std::cout << "Config File Open Failed" << std::endl;
		return ;
	}
	while ((n = read(fd, buf, 29)) > 0)
	{
		buf[n] = 0;
		for (idx=0;idx<n;++idx)
		{
			if (buf[idx] == '\n')
			{
				token.push_back(string());
				++vdx;
			}
			else
				token[vdx].push_back(buf[idx]);
		}
	}
	close(fd);
}

//------------------------------------------------------------------------------

void	Webserver::server_create(deque<string>& token)
{
	string::iterator	it;


	while (!token.empty())
	{
		it = token[0].begin();
		if (!token.empty() && !ft::strncmp(it.base(), "server", 6) && token[0].length() == 6)
		{
			token.pop_front();
			Server 	temp(token);
			server_iterator	server_it = servers.find(temp.port);
			if (server_it == servers.end())
				servers[temp.port] = map<string, Server>();
			servers[temp.port].insert(make_pair(temp.name, temp));
		}
		else
			throw Webserver::InvalidServerBlock();
	}
}

//------------------------------------------------------------------------------

void	Webserver::create_sockets()
{
	server_iterator it = servers.begin();
	server_iterator end = servers.end();

	sockets.reserve(servers.size());
	while (it != end)
	{
		sockets.push_back(Socket());
		sockets.back().bind(it->first, INADDR_ANY);

		#ifdef DBG
		cout << "socket: " << sockets.back().fd << ":" << it->first << endl;
		#endif
		
		o_set.set(sockets.back().fd);
		++it;
	}
}

//------------------------------------------------------------------------------

void			Webserver::start_server()
{
	int		result;

	socket_iterator	it = sockets.begin();
	socket_iterator	end = sockets.end();

	#ifdef DBG
	cout << __func__ << endl;
	#endif

	while (it != end)
	{
		it->listen(max_connection);
		++it;
	}
	while (42)
	{
		cout << endl;
		cout << "################################################################################" << endl;
		cout << endl;
		usleep(200000);
		r_set = o_set;
		w_set = o_set;
		e_set = o_set;
		select_timeout.set_time(2000000);
		result = select(max_connection, &r_set.bits, &w_set.bits, &e_set.bits, (&select_timeout));
		#ifdef DBG
		cout << "select loop" << endl;
		cout << "o:" << endl;
		o_set.print_bit(20);
		cout << "r:" << endl;
		r_set.print_bit(20);
		#endif
		if (result < 0)
			throw SelectFailed();
		if (result == 0)	// timeout
		{
			#ifdef DBG
			cout << "timeout" << endl;
			#endif
			continue;
		}
		check_new_connection();
		manage_clients();
	}
}

//------------------------------------------------------------------------------

void			Webserver::check_new_connection()
{
	socket_iterator		it = sockets.begin();
	socket_iterator		end = sockets.end();
	cout << '\n' <<  __func__ << "\n======================================"<< endl;
	while (it != end)
	{
		if (r_set.get(it->fd))
		{
			#ifdef DBG
			cout << "- new connection with " << it->fd << endl;
			cout << "- port: " << ft::hton(it->s_addr.sin_port) << endl;
			#endif

			clients.push_back(servers[ft::hton(it->s_addr.sin_port)]);
			
			#ifdef DBG
			cout << "- push_back\n";
			#endif

			clients.back().init(it->fd);
			
			#ifdef DBG
			cout << "- init\n";
			#endif
			
			o_set.set(clients.back().sock.fd);
			fcntl(clients.back().sock.fd, O_NONBLOCK);
			
			#ifdef DBG
			cout << "- new client " << clients.back().sock.fd << endl;
			cout << endl;
			#endif
		}
		if (e_set.get(it->fd))
		{
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		++it;
	}
}

//------------------------------------------------------------------------------

void			Webserver::manage_clients()
{
	#ifdef DBG
	cout << '\n' << __func__ << "\n======================================"<< endl;
	#endif

	for (client_iterator it = clients.begin() ; it != clients.end() ; ++it)
	{
		if (e_set.get(it->sock.fd))
		{
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		cout << "----------------\n";
		cout << "== Socket: " << it->sock.fd << " ==" << endl;
		cout << "----------------\n";
		it->client_process(r_set, w_set);
		if (it->status == STATUS_DONE)
		{
			cout << "- delete " << it->sock.fd << endl;
			cout << "- erase " << it->sock.fd << endl;
			o_set.del(it->sock.fd);
			it = clients.erase(it);
		}
	}
}

//------------------------------------------------------------------------------

const char*	Webserver::InvalidServerBlock::what() const throw()
{
	return ("Server Block start line name Invalid");
}

//------------------------------------------------------------------------------

const char*	Webserver::SelectFailed::what() const throw()
{
	return ("Select Failed");
}

//------------------------------------------------------------------------------

void		Webserver::set_path_cgi_bin(char** env)
{
	Cgi::cgi_bin["php"] = ft::which("php", env);
	Cgi::cgi_bin["py"] = ft::which("python3", env);
}

//------------------------------------------------------------------------------

std::ostream&	operator<<(std::ostream& os, Webserver& ref) {
	Webserver::server_iterator		it = ref.servers.begin();
	Webserver::server_iterator		end = ref.servers.end();
	
	for ( ; it != end ; ++it)
	{
		for (map<string, Server>::iterator  it_s = it->second.begin() ; it_s != it->second.end() ; ++it_s)
		{
			os << it_s->second;
		}
	}
	return (os);
}
