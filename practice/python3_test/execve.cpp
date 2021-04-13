#include <unistd.h>
#include <stdlib.h>
#include <iostream>

int		main()
{
	char**	env = new char*[6];
	char	a[] = "AA=aa";
	char	b[] = "BB=bb";
	char	c[] = "CC=cc";
	char	d[] = "DD=dd";
	char	e[] = "EE=ee";
	env[0] = a;
	env[1] = b;
	env[2] = c;
	env[3] = d;
	env[4] = e;
	env[5] = 0;
	std::cout << "hi\n" ;

	char**	argv = new char*[2];
	char	p[] = "env.py";
	argv[0] = p;
	argv[1] = 0;
	std::cout << argv[0] << std::endl;
	std::cout << execve("/usr/bin/python3", argv, env) << std::endl;
	// std::cout << execve("env.bin", argv, env) << std::endl;
}