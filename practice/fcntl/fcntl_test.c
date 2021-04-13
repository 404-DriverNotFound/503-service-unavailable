#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int		main(void)
{
	char	buffer[30];

	fcntl(0, F_SETFL, O_NONBLOCK);
	read(0, buffer, 5);
}