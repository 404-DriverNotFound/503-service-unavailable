#pragma once
#include <iostream>
using namespace std;
class	bar;
class	fooA;
class	fooB;
class	fooC;

class	foo
{
	protected:
		static fooA* 	foo_a;
		static fooB* 	foo_b;
		static fooC* 	foo_c;

	public:
		static fooA*	get_a();
		static fooB*	get_b();
		static fooC*	get_c();
		virtual foo*	handle(bar* x) = 0;
};