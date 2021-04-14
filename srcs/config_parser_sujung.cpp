#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "utils.hpp"
// #include "Webserver.hpp"
// #include "Server.hpp"
// #include "ConfigWebserver.hpp"
#include "Config.hpp"

// void	set_conf(Config& conf, std::vector<std::string>& token)
// {
// 	for (int idx=0;idx<token.size();idx++)
// 	{
// 		tok
// 	}
// }

void	config_parser(Config& conf)
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
			idx++;
			token.push_back(buf+idx);
		}
	}
	// for (int i=0;i<token.size();i++)
	// 	std::cout << token[i] << "\n";
	close(fd);
}

// int		main(void) {
// 	Config	conf;
// 	config_parser(conf);
// }