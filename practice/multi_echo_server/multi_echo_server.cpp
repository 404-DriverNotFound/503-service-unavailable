#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUFFER_SIZE 30
void	error_handling(const char *str)
{
	std::cout << str << " error" << std::endl;
	exit(1);
}

int		main(int argc, char **argv)
{
	int				serv_sock;
	int				clnt_sock;
	sockaddr_in		serv_addr;
	sockaddr_in		clnt_addr;
	timeval			timeout;
	fd_set			reads;
	fd_set			cpy_reads;
	socklen_t		addr_size;
	int				fd_max;
	int				len;
	int				fd_num;
	int				i;
	char			buffer[BUFFER_SIZE];

	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << "<port>\n";
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
		error_handling("bind()");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen()");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
		{
			std::cout << "select()\n";
			break ;
		}
		if (fd_num == 0)
		{
			std::cout << "timeout\n";
			continue;
		}

		for (i = serv_sock ; i < fd_max + 1 ; i++)
		{
			std::string		msg_recv;

			if (FD_ISSET(i, &cpy_reads))
			{
				if (i == serv_sock)
				{
					addr_size = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &addr_size);
					FD_SET(clnt_sock, &reads);
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					std::cout << "connected client: " << clnt_sock << std::endl;
				}
				else
				{
					int		read_len;
					read_len = read(i, &len, sizeof(int));
					if (read_len == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						std::cout << "close client: " << i << std::endl;
					}
					else
					{
						len = read(i, buffer, BUFFER_SIZE);
						buffer[len] = 0;
						std::cout << "Message from client: " << buffer << std::endl;
						write(i, &len, sizeof(int));
						write(i, buffer, len);
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}