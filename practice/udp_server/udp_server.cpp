#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define BUFFER_SIZE 32

void	error_handling(char const* s)
{
	std::cout << s << "error" << std::endl;
	exit(1);
}

int		main(int argc, char **argv)
{
	int				serv_sock;
	char			msg[BUFFER_SIZE + 1];
	int				len;
	socklen_t		clnt_addr_size;
	sockaddr_in		serv_addr;
	sockaddr_in		clnt_addr;

	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << "<port>" << std::endl;
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1)
		error_handling("bind()");
	
	while (1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		len = recvfrom(serv_sock, msg, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
		msg[len] = 0;
		std::cout << "recv msg: " << msg << std::endl;
		sendto(serv_sock, msg, len, 0, reinterpret_cast<sockaddr*>(&clnt_addr), clnt_addr_size);
	}
	close(serv_sock);
	return 0;
}