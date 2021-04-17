#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int		main(void)
{
	char	buffer[1000];
	int	fd = open("file0", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	int 	len;
	while ((len = read(0, buffer, 1000)))
	{
		buffer[len] = 0;
		write(fd, buffer, len);
	}

	// if (fd < 0)
	// 	std::cout << "fajsdlfkjhas\n";
	// write(fd, "asdf", 4);
	close(fd);
}