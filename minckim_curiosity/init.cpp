#include <iostream>

void		foo(int a, int b = 70);

void		foo(int a, int b = 9000)
{
	std::cout << a << std::endl;
	std::cout << b << std::endl;
}

int			main()
{
	foo(10);
}