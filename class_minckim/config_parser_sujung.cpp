#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "utils.hpp"
// #include "Webserver.hpp"
// #include "Server.hpp"
// #include "ConfigWebserver.hpp"
#include "Config.hpp"

void	set_conf(std::vector<std::string>& token)
{
	std::string::iterator	it;
	std::string				temp;
	for (int idx=0;idx<token.size();idx++)
	{
		it = token[idx].begin();
		if (std::strncmp(it.base(), "\t\t", 2))
		{

		}
		else if (std::strncmp(it.base(), "\t", 1))
		{

		}
		else
		{
			
		}
	}
}

void	config_parser()
{
	std::vector<std::string>	token(1);
	char						*p;
	char						buf[30];
	int							fd, n, idx, vdx = 0;

	if ((fd = open("./server_config", O_RDONLY)) < 0)
	{
		std::cout << "Config File Open Failed" << std::endl;
		return ;
	}
	while ((n = read(fd, buf, 29)) > 0)
	{
		buf[n] = 0;
		p = strchr(buf, '\n');
		if (!p)
			token[vdx] += buf;
		else
		{
			for (idx=0;idx<p - buf;idx++)
				token[vdx].push_back(buf[idx]);
			vdx++;
			token.push_back(buf+ ++idx);
		}
	}
	for (int i=0;i<token.size();i++)
		std::cout << token[i] << std::endl;
	close(fd);
}

int		main(void) {
	// Config	conf;
	config_parser();
}