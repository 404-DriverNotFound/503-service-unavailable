#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Server_config_sujung.hpp"
#include "config_parser_sujung.cpp"

Server_Config::Server_Config()
{
	std::vector<std::string>	token(1);
	config_parser(token);
	for (int idx=0;idx<token.size();idx++)
		std::cout << token[idx] << std::endl;
}

int		main()
{
	Server_Config	s;
}