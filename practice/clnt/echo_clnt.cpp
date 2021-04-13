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


	while (1)
	{
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


		char			buffer[BUFFER_SIZE + 1];
		int				len = 0;
		std::string		msg_send;
		std::string		msg_recv;

		std::cout << "msg_send: ";
		std::getline(std::cin, msg_send);
		if (std::cin.eof() || msg_send.length() == 0)
		{
			std::cout << "Disconnect\n";
			close(sock);
			break ;
		}
		len = msg_send.length();
		write(sock, msg_send.c_str(), len);
		write(sock, "\0", 1);

		while ((len = read(sock, buffer, BUFFER_SIZE)))
		{
			if (len == -1)
			{
				std::cout << "here??\n";
				error_handling("read()");
			}
			buffer[len] = 0;
			msg_recv.append(buffer);
		}
		std::cout << "Message from server: " << msg_recv << std::endl;
	
		close(sock);
	}
	return 0;
}