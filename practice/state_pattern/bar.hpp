#pragma once
class	bar;
#include "foo.hpp"
#include "fooA.hpp"
#include "fooB.hpp"
#include "fooC.hpp"
class	bar
{
	foo&	state;

	public:
		bar(foo& x);
		void	set_state(fooA&	foo);
		void	set_state(fooB&	foo);
		void	set_state(fooC&	foo);
		void	run();
};