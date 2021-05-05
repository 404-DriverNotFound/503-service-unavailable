#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "../includes/Webserver.hpp"
#define DBG

/*#####################################
Webserver
######################################*/


void	broken(int code) {
	(void)code;
	perror("ERROR : ");
	exit(1);
}

Webserver::Webserver(int argc, char** argv, char** env)
: max_connection(100), select_timeout(5000000)
{
	if (argc > 2)
	{
		std::cout << "Usage: " << argv[0] << " [configfile]" << std::endl;
		return ;
	}
	Cgi::set_path_cgi_bin(env);
	HttpRes::init_status_code();
	Http::init_map_headers();
	Http::init_map_methods();
	Method::init_method_num();
	Method::init_method_flags();
	Method::init_method_strings();
	deque<std::string>	lines(1);
	
	signal(SIGPIPE, broken);
	signal(SIGCHLD, SIG_IGN);
	// signal(SIGPIPE, SIG_IGN);

	config_parser(lines, argv[1]);
	create_server(lines);
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
{
	socket_iterator it = sockets.begin();
	socket_iterator end = sockets.end();
	while (it != end)
	{
		delete *it;
		++it;
	}
}

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

void	Webserver::create_server(deque<string>& lines)
{
	while (!lines.empty())
	{
		int		indent = ft::count_chr(lines.front(), '\t');
		lines.front().erase(lines.front().begin(), lines.front().begin() + indent);
		list<string>	tokens = ft::get_set_token(lines.front(), " ");

		if (tokens.empty())
		{
			tokens.pop_front();
			continue;
		}
		if (tokens.front() == "server")
		{
			lines.pop_front();
			Server	tmp(lines);
			server_iterator	server_it = servers.find(tmp.port);
			if (server_it == servers.end())
				servers[tmp.port] = map<string, Server>();
			servers[tmp.port].insert(make_pair(tmp.name, tmp));
			if (lines.empty())
				break;
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
		// sockets.push_back(Socket());
		sockets.push_back(new Socket(it->first, INADDR_ANY));

		#ifdef DBG
		cout << "socket: " << sockets.back()->fd << ":" << it->first << endl;
		#endif
		
		o_set.set(sockets.back()->fd);
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
		(*it)->listen(max_connection);
		++it;
	}
	while (42)
	{
		cout << endl;
		cout << "################################################################################" << endl;
		cout << endl;
		// usleep(1000);
		r_set = o_set;
		w_set = o_set;
		e_set = o_set;
		select_timeout.set_time(2000000);
		result = select(max_connection, &r_set.bits, &w_set.bits, &e_set.bits, (&select_timeout));
		#ifdef DBG
		// cout << "select loop" << endl;
		// cout << "o: ";
		// o_set.print_bit(20);
		// cout << "r: ";
		// r_set.print_bit(20);
		// cout << "w: ";
		// w_set.print_bit(20);
		// cout << "e: ";
		// e_set.print_bit(20);
		#endif
		if (result < 0)
		{
			perror("func: start server : ");
			throw SelectFailed();
		}
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
	// cout << '\n' <<  __func__ << "\n======================================"<< endl;
	while (it != end)
	{
		if (r_set.get((*it)->fd))
		{
			// #ifdef DBG
			// cout << "- new connection with " << it->fd << endl;
			// cout << "- port: " << ft::hton(it->s_addr.sin_port) << endl;
			// #endif
			clients.push_back(new Client((*it)->fd, servers[ft::hton((*it)->s_addr.sin_port)], r_set, w_set));
			
			// #ifdef DBG
			// cout << "- push_back\n";
			// #endif
			
			// #ifdef DBG
			// cout << "- init\n";
			// #endif
			
			o_set.set(clients.back()->sock.fd);
			// fcntl(clients.back().sock.fd, O_NONBLOCK);
			
			// #ifdef DBG
			// cout << "- new client " << clients.back().sock.fd << endl;
			// cout << endl;
			// #endif
		}
		if (e_set.get((*it)->fd))
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

	for (client_iterator it = clients.begin() ; it != clients.end() ; ++it)
	{
		if (e_set.get((*it)->sock.fd))
		{
			throw SelectFailed();	// TODO: 아무튼 fd에 이상이 생긴것. 새로운 예외클래스 추가
		}
		cout << "----------------\n";
		cout << "== Socket: " << (*it)->sock.fd << " ==" << endl;
		cout << "----------------\n";
		(*it)->process();
		if ((*it)->status == CLIENT_DONE)
		{
			cout << "- delete " << (*it)->sock.fd << endl;
			o_set.del((*it)->sock.fd);
			// close((*it)->sock.fd);
			delete *it;
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
