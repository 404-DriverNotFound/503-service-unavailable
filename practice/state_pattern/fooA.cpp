#include "fooA.hpp"

void		fooA::handle(bar& x)
{
	cout << "a\n";
	x.set_state(this->foo_b);
}