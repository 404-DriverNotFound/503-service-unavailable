#pragma once
class	foo;
#include "fooA.hpp"
#include "fooB.hpp"
#include "fooC.hpp"
#include <iostream>
using namespace std;
class	foo
{
	protected:
		static fooA foo_a;
		static fooB foo_b;
		static fooC foo_c;

	public:
		static fooA&	get_a();
		static fooB&	get_b();
		static fooC&	get_c();
		virtual void	handle(bar& x) = 0;
};