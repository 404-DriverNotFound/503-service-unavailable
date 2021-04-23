#include <iostream>
#include "Webserver.hpp"

int		main(int argc, char** argv, char** env)
{
	try {
		Webserver	s(argc, argv, env);
<<<<<<< HEAD
		Webserver::server_iterator	end = s.servers.end();
		for (Webserver::server_iterator it = s.servers.begin();it != end;++it)
			std::cout << it->second << std::endl;
=======
		s.start_server();
>>>>>>> d5468caa492896680405b949d1e9538ea41cfdb3
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
