#include "../includes/Webserver.hpp"
#include "../includes/Bonus.hpp"
#include "../includes/color.h"

void*	run_webserver(void* ptr_webserver)
{
	Webserver*	webserver = reinterpret_cast<Webserver*>(ptr_webserver);
	webserver->start_server();
	return 0;
}

void	change_server_state(int code)
{
	(void)code;
	write(1, "\b\b", 2);
	Webserver::server_state = false;
}

void	print_ascii_art(const string& path)
{
	File		file(path, File::o_read);
	Stream		stream(10000, file.get_fd());
	while (stream.fill(10000));
	stream.print();
}

#if __BONUS__ == 1
void	bonus_routine()
{
	pthread_t*	worker = new pthread_t[Webserver::config.get_worker()];
	Webserver*	webserver = new Webserver[Webserver::config.get_worker()];
	for (int i = 0 ; i < Webserver::config.get_worker() ; ++i)
	{
		webserver[i].worker_serial = i;
		pthread_create(&worker[i], 0, run_webserver, &webserver[i]);
	}
	for (int i = 0 ; i < Webserver::config.get_worker() ; ++i)
	{
		pthread_join(worker[i], 0);
	}
}
#endif

int		main(int argc, char** argv, char** env)
{
	print_ascii_art(string("./asciiart"));
	signal(SIGINT, change_server_state);
	try
	{
		Webserver::init_static_members(argc, argv, env);
		#if __BONUS__ == 1
			bonus_routine();
		#else
			Webserver().start_server();
		#endif
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		Webserver::destroy_static_members();
		return 1;
	}
	std::cout << GREEN << "server stopping..." << WHITE << std::endl;
	Webserver::destroy_static_members();
	return 0;
}