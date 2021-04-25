#include <iostream>
#include "../includes/Webserver.hpp"

int		main(int argc, char** argv, char** env)
{
	try {
		Webserver	s(argc, argv, env);

		// Webserver::server_iterator	end = s.servers.end();
		// for (Webserver::server_iterator it = s.servers.begin();it != end;++it)
		// 	std::cout << it->second << std::endl;
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
