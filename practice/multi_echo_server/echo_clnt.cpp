#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFFER_SIZE 30

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
		std::cout << "Usage: " << argv[0] << " <IP> <port>\n";
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

	while (1)
	{
		char			buffer[BUFFER_SIZE + 1];
		int				len = 0;
		std::string		msg_send;
		std::string		msg_recv;

		std::getline(std::cin, msg_send);
		if (std::cin.eof() || msg_send.length() == 0)
		{
			std::cout << "len == 0\n";
			break ;
		}
		len = msg_send.length();
		write(sock, &len, sizeof(int));
		write(sock, msg_send.c_str(), len);
		read(sock, &len, sizeof(int));
		std::cout << len << std::endl;
		read(sock, buffer, len);
		buffer[len] = 0;
		std::cout << "here\n";
		std::cout << "Message from server: " << buffer << std::endl;
	}
	close(sock);
	return 0;
}