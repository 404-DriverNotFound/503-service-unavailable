#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUFFER_SIZE 30

int		main(int argc, char **argv)
{
	fd_set	reads;
	fd_set	tmps;
	int		result;
	int		len;
	char	buffer[BUFFER_SIZE];
	timeval	timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);

	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;

	while (1)
	{
		tmps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1, &tmps, 0, 0, &timeout);
		if (result == -1)
		{
			std::cout << "select() error\n";
			break ;
		}
		else if (result == 0)
		{
			std::cout << "timeout\n";
		}
		else
		{
			if (FD_ISSET(0, &tmps))
			{
				len = read(0, buffer, BUFFER_SIZE);
				buffer[len] = 0;
				std::cout << "msg from console: " << buffer << std::endl;
			}
		}
	}
	return 0;
}