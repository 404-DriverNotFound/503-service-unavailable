#include "fooB.hpp"
#include "bar.hpp"

fooB*	foo::foo_b = new fooB;

void		fooB::handle(bar* x)
{
	cout << "b\n";
	x->set_state(this->foo_c);
}