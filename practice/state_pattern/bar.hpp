#pragma once
#include <iostream>

using namespace std;
class	foo;
class	fooA;
class	fooB;
class	fooC;

class	bar
{

		foo*	state;
	public:
		bar(foo* x);
		void	run();
};