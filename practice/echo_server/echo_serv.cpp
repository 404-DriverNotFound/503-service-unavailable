#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <vector>

#define BUFFER_SIZE 16

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
	
	for (int i = 0 ; i < 5 ; i++)
	{
		int					len_max;
		int					curr_len = 0;
		int					len = 0;
		std::string			str;
		char				buffer[BUFFER_SIZE + 1];

		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept()");
		if (read(clnt_sock, &len_max, sizeof(int)) == -1)
			error_handling("read()");
		std::cout << "len max: " << len_max << std::endl;
		str.reserve(len_max + 1);
		while ((len = read(clnt_sock, buffer, BUFFER_SIZE)))
		{
			if (len == -1)
				error_handling("read()");
			std::cout << "recv len: " << len << std::endl;
			buffer[len] = 0;
			std::cout << "recv msg: " << buffer << std::endl;
			str.append(buffer);
			if (str.length() >= len_max)
				break ;
		}
		std::cout << "outofloop\n";
		len = str.length();
		write(clnt_sock, &len, sizeof(int));
		for (int i = 0 ; i < len_max ; i++)
			write(clnt_sock, &str[i], 1);
		close(clnt_sock);
		usleep(4000000);
	}


	close(serv_sock);
	return 0;
}