#include "fooC.hpp"

void		fooA::handle(bar& x)
{
	cout << "c\n";
	x.set_state(this->foo_a);
}