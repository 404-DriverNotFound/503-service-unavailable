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
		void	set_state(fooA*	foo);
		void	set_state(fooB*	foo);
		void	set_state(fooC*	foo);
		void	run();
};