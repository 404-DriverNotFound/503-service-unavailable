#include <iostream>
#include "Webserv_sujung.hpp"

int		main(void)
{
	try {
		WebServ	s;
		for (int i=0;i<s.server.size();i++)
			std::cout << s.server[i].server_name << std::endl;
	}
	catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}