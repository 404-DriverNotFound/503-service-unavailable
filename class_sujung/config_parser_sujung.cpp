#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "Server_config_sujung.hpp"

void	set_conf(std::vector<std::string>& token)
{
	std::string::iterator	it;
	bool					flag = false;

	for (int idx=0;idx<token.size();idx++)
	{
		it = token[idx].begin();
		if (flag && !std::strncmp(it.base(), "\t\t", 2))
		{
			it += 2;
		}
		else if (!flag && !std::strncmp(it.base(), "\t", 1))
		{
			it += 1;
		}
		else
		{
			
		}
	}
}

void	config_parser(std::vector<std::string> &token)
{
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
	close(fd);
}

// int		main(void) {
// 	// Config	conf;
// 	config_parser();
// }