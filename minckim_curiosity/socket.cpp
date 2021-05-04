#include <unistd.h>
#include <fcntl.h>
#include <iostream>

using std::cout;
using std::endl;

int		main()
{
	int		fd0 = open("txt0", O_CREAT | O_RDWR, 0644);
	int		len;

	int		fd1 = dup(fd0);
	int		fd2 = dup(fd0);

	cout << fd0 << endl;
	cout << fd1 << endl;
	cout << fd2 << endl;

	char	buffer[10];
	
	len = read(fd0, buffer, 5);
	buffer[len] = 0;
	cout << buffer << endl;

	len = read(fd1, buffer, 5);
	buffer[len] = 0;
	cout << buffer << endl;
	
	len = read(fd2, buffer, 5);
	buffer[len] = 0;
	cout << buffer << endl;

	close(fd0);
	int		fdp[2];
	pipe(fdp);
	cout << fdp[0] << " " << fdp[1] << endl;

	write(fd1, "this", 4);
	write(fd2, " is", 3);
	close(fd1);
	// close(fd2);
	write(fd1, "aaaa", 4);
	write(fd2, "bbbb", 4);
}