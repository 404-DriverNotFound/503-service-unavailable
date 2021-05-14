#include "fooA.hpp"
#include "bar.hpp"

fooA*	foo::foo_a = new fooA;

void		fooA::handle(bar* x)
{
	cout << "a\n";
	x->set_state(this->foo_b);
}