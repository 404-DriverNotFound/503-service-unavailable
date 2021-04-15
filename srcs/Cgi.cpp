#include "Cgi.hpp"
// #include <cstdio>
#include <iostream>

			Cgi::Cgi()
: path(0), meta_variable(0), fd_out(fd_pipe[1])
{}

void		Cgi::init(char* path, char** meta_variable)
{
	path = path;
	meta_variable = meta_variable;
	pipe(fd_pipe);
}

void		Cgi::start_cgi()
{
	pid = fork();

	if (pid == 0)
	{
		std::cout << "fork\n";
		dup2(fd_pipe[0], 0);
		close(fd_pipe[1]);
		if (execve(path, 0, meta_variable))
			std::cout << "fxxxx\n" << path << std::endl;
	}
	else
	{
		close(fd_pipe[0]);
	}
}

void		Cgi::destroy_pipe()
{
	close(fd_pipe[1]);
}

/*
int			main(void)
{
	Cgi		c0("exe0", 0);
	// Cgi		c1("./exe1");
	c0.start_cgi();
	c0.connect();
	// printf("this must be in file0\n");
	// std::cout << "this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0";
	// std::cout << "this must be in file0" << std::endl;
	// write(1, "this must be in file0\n", strlen("this must be in file0\n"));
	// for (int i = 0 ; i < 80000 ;)
	// {
	// 	write(1, "0123456789abcdef\n", 17);
	// 	i += 11;
	// 	// usleep(100000);
	// }
	char*	str = new char[7000];
	memset(str, 'a', 7000);
	write(1, str, 7000);
	write(1, "\n", 1);
	c0.disconnect();
	printf("this must be in test0\n");
	
	
	c0.connect();
	memset(str, 'a', 7000);
	write(1, str, 7000);
	write(1, "\n", 1);
	c0.disconnect();
	// std::cout << "this must be in stdout" << std::endl;
	// write(1, "this must be in stdout\n", strlen("this must be in stdout\n"));
	c0.destroy_pipe();
}
*/