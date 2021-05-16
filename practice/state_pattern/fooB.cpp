#include "fooB.hpp"
#include "fooC.hpp"
#include "bar.hpp"

fooB*	foo::foo_b = new fooB;

foo*		fooB::handle(bar* x)
{
	cout << "b\n";
	return foo_c;
}