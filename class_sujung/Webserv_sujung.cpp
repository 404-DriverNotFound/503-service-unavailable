#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Webserv_sujung.hpp"

WebServ::WebServ()
{
	std::deque<std::string>	token(1);
	config_parser(token);
	server_create(token);
}

WebServ::~WebServ()
{
}

void	WebServ::config_parser(std::deque<std::string>& token)
{
	char						*base;
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
		for (int idx=0;idx<n;++idx)
		{
			if (buf[idx] == '\n')
			{
				token.push_back(std::string());
				++vdx;
			}
			else
				token[vdx].push_back(buf[idx]);
		}
	}
	close(fd);
}

void	WebServ::server_create(std::deque<std::string>& token)
{
	std::string::iterator	it;


	while (!token.empty())
	{
		it = token[0].begin();
		if (!std::strncmp(it.base(), "server", 6) && token[0].length() == 6)
		{
			token.pop_front();
			server.push_back(Server(token));
		}
		else
			throw WebServ::InvalidServerBlock();
	}
}

const char*	WebServ::InvalidServerBlock::what() const throw()
{
	return ("Server Block start line name Invalid");
}

std::ostream&	operator<<(std::ostream& os, WebServ& ref) {
	return (os);
}