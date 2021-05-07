#include <map>
#include <iostream>
using std::map;

int		main()
{
	map<int, int>	m;

	m.insert(std::make_pair(1, 2));
	m.insert(std::make_pair(2, 4));

	// std::cout << m[1] << std::endl;
	// std::cout << m[2] << std::endl;
	// std::cout << m[10] << std::endl;
	if (m.find(1) == m.end())
		std::cout << "1: non\n";
	if (m.find(2) == m.end())
		std::cout << "2: non\n";
	if (m.find(10) == m.end())
		std::cout << "10: non\n";
}