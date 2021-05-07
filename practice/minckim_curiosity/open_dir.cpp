#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include <sys/stat.h>
using std::cout;
using std::endl;

int	main()
{
	// int		fd = open("./", O_RDONLY);
	// cout << fd << endl;

	{
		struct stat	stat_f;
		stat("./", &stat_f);
		cout << stat_f.st_mode << endl;
	}
	{
		struct stat	stat_f;
		stat("map_test.cpp", &stat_f);
		cout << stat_f.st_mode << endl;
	}
	{
		struct stat	stat_f;
		stat("./", &stat_f);
		cout << stat_f.st_mode << endl;
	}
	{
		struct stat	stat_f;
		stat("map_test.cpp", &stat_f);
		cout << stat_f.st_mode << endl;
	}
}