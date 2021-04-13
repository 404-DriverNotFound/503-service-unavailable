#include <sys/socket.h>
#include <iostream>

int		main(void)
{
	int			sock;
	int			send_buff_size;
	int			recv_buff_size;
	socklen_t	len;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	len = sizeof(send_buff_size);
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&send_buff_size, &len);
	std::cout << "buff size: " << send_buff_size << std::endl;

	len = sizeof(recv_buff_size);
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&send_buff_size, &len);
	std::cout << "recv size: " << send_buff_size << std::endl;
	return 0;
}