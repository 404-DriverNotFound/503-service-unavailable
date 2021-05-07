#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 8

void	error_handling(char const* s)
{
	std::cout << s << "error" << std::endl;
	exit(1);
}

int		main(int argc, char **argv)
{
	int					sock;
	sockaddr_in			serv_addr;

	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << "<IP> <port>\n";
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket()");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
		error_handling("connet()");
	else
		std::cout << "connected!\n";

	{
		write(sock, "00000", 5);
		usleep(600000);
		write(sock, "11111", 5);
		usleep(600000);
		write(sock, "22222", 5);
		usleep(600000);
	}
	close(sock);
	return 0;
}