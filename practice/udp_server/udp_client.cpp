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
	char			msg[BUFFER_SIZE];
	int				len;
	sockaddr_in		serv_addr;
	sockaddr_in		from_addr;
	socklen_t		from_addr_size;

	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << "<port>" << std::endl;
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1)
		error_handling("UDP socket creation error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	while (1)
	{
		std::string		str;

		std::cout << "Insert message: ";
		std::getline(std::cin, str);
		if (std::cin.eof())
			break ;
		if (str == "q")
			break ;
		
		sendto(serv_sock, str.c_str(), str.length(), 0, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
		from_addr_size = sizeof(from_addr);
		len = recvfrom(serv_sock, msg, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&from_addr), &from_addr_size);
		msg[len] = 0;
		std::cout << "recv msg: " << msg << std::endl;
	}
	close(serv_sock);
	return 0;
}