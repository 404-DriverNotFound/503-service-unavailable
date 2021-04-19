#include <iostream>
#include "Webserver.hpp"

int		main(int argc, char** argv, char** env)
{
	try {
		Webserver	s(argc, argv, env);
		for (size_t i=0;i<s.servers.size();i++)
			std::cout << s.servers[i] << std::endl;
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
