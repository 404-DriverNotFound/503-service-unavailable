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
	std::string			msg;
	std::string			str;

	if (argc != 4)
	{
		std::cout << "Usage: " << argv[0] << "<IP> <port>\n";
		exit(1);
	}
	str = argv[3];

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
		int				len = 0;
		int				len_max;
		char			buffer[BUFFER_SIZE + 1];
		std::string		msg;

		len_max = str.length();
		write(sock, &len_max, sizeof(int));
		for (int i = 0 ; i < len_max ; i++)
			write(sock, &str[i], 1);
		read(sock, &len_max, sizeof(int));
		std::cout << "len max: " << len_max << std::endl;
		while ((len = read(sock, buffer, BUFFER_SIZE)))
		{
			if (len == -1)
				error_handling("read()");
			buffer[len] = 0;
			msg.append(buffer);
		}
		std::cout << "Message from server: " << msg << std::endl;
	}
	close(sock);
	return 0;
}