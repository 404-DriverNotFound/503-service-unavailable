#include "fooC.hpp"
#include "bar.hpp"

fooC*	foo::foo_c = new fooC;

void		fooC::handle(bar* x)
{
	cout << "c\n";
	x->set_state(this->foo_a);
}