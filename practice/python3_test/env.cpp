#include <iostream>

int		main(int argc, char** argv, char** env)
{
	while (*env)
		std::cout << *env++ << std::endl;
}