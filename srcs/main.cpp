#include <iostream>
#include "../includes/Webserver.hpp"

int		main(int argc, char** argv, char** env)
{
	try {
		Webserver	s(argc, argv, env);
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
