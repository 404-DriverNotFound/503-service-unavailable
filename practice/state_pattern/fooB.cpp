#include "fooB.hpp"

void		fooB::handle(bar& x)
{
	cout << "b\n";
	x.set_state(this->foo_c);
}