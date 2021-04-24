#include "Socket.hpp"
#include <cstring>
#include <fcntl.h>
#include <sys/select.h>
#define BUFFER 1000

int		main()
{
	Socket					server(9190, INADDR_ANY);
	fd_set					r_set;
	fd_set					w_set;
	fd_set					e_set;
	fd_set					o_set;
	timeval					timeout;
	std::list<Socket>		clients;
	std::list<std::string>	buffers;
	char					buffer[BUFFER + 1];
	int						len;

	FD_ZERO(&o_set);
	FD_SET(server.fd, &o_set);

	server.listen(5);
	std::cout << "listen..." <<std::endl;
	std::cout << "server fd: " << server.fd <<std::endl;
	std::cout << "server port: " << htons(server.s_addr.sin_port) <<std::endl;
	while (42)
	{
		r_set = o_set;
		w_set = o_set;
		e_set = o_set;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (select(100, &r_set, &w_set, &e_set, &timeout) == 0)
		{
			std::cout << "timeout" << std::endl;
			continue ;
		}
		std::cout << "select..." <<std::endl;
		usleep(1000000);
		if (FD_ISSET(server.fd, &r_set))
		{
			clients.push_back(Socket());
			clients.back().accept(server.fd);
			buffers.push_back(std::string());
			std::cout << "connected client: " << clients.back().fd <<std::endl;
			fcntl(clients.back().fd, O_NONBLOCK);
			FD_SET(clients.back().fd, &o_set);
		}

		std::list<Socket>::iterator 		it = clients.begin();
		std::list<std::string>::iterator	it_b = buffers.begin();
		while (it != clients.end())
		{
			std::cout << "iterator fd:" << it->fd << std::endl;
			if (FD_ISSET(it->fd, &r_set))
			{
				std::cout << "read fd:" << it->fd;
				len = read(it->fd, buffer, BUFFER);
				std::cout << " [" << len << "] \n";
				buffer[len] = 0;
				write(1, buffer, len);
				*it_b += buffer;
			}
			if (!FD_ISSET(it->fd, &r_set) && FD_ISSET(it->fd, &w_set))
			{
				std::cout << "write fd:" << it->fd << std::endl;
				write(it->fd, it_b->c_str(), it_b->length());
			}
			if (!FD_ISSET(it->fd, &r_set))
			{
				std::cout << "close fd:" << it->fd << std::endl;
				close(it->fd);
				FD_CLR(it->fd, &o_set);
			}
			++it;
			++it_b;
		}
	}
}


	// server.listen(5);

	// Socket					client;
	// std::cout << server.fd << std::endl;
	// client.accept(server.fd);
	// // len = read(client.fd, buffer, BUFFER);
	// std::cout << len << std::endl;
	// write(1, buffer, len);
