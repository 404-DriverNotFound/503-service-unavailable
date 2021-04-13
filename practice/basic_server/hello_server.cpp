#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

void	error_handling(char const* s)
{
	std::cout << s << "error" << std::endl;
	exit(1);
}

int		main(int argc, char **argv)
{
	int					serv_sock;
	int					clnt_sock;
	sockaddr_in			serv_addr;
	sockaddr_in			clnt_addr;
	socklen_t			clnt_addr_size;
	char				msg[] = "Hello World!";

	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << "<port>\n";
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket()");

	memset(&serv_addr, 0, sizeof(sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
		error_handling("bind()");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen()");
	
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
	if (clnt_sock == -1)
		error_handling("accept()");
	
	write(clnt_sock, msg, sizeof(msg));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}