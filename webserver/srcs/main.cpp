#include "../includes/Webserver.hpp"
#include "../includes/Bonus.hpp"

void*	run_webserver(void* ptr_webserver)
{
	Webserver*	webserver = reinterpret_cast<Webserver*>(ptr_webserver);
	webserver->start_server();
	return 0;
}

int		main(int argc, char** argv, char** env)
{
	try
	{
		Webserver::init_static_members(argc, argv, env);
		cout << Webserver::config << endl;

		#ifdef __BONUS__
		pthread_t*	worker = new pthread_t[Webserver::config.get_worker()];
		Webserver*	webserver = new Webserver[Webserver::config.get_worker()];
		for (int i = 0 ; i < Webserver::config.get_worker() ; ++i)
		{
			pthread_create(&worker[i], 0, run_webserver, &webserver[i]);
		}
		for (int i = 0 ; i < Webserver::config.get_worker() ; ++i)
		{
			pthread_join(worker[i], 0);
		}
		#else
		Webserver().start_server();
		#endif
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	return 0;
}