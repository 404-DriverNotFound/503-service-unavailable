#include <iostream>
#include "Webserv_sujung.hpp"

int		main(void)
{
	try {
		WebServ	s;
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}