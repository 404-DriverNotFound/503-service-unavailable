#include "fooA.hpp"
#include "fooB.hpp"
#include "bar.hpp"

fooA*	foo::foo_a = new fooA;

foo*		fooA::handle(bar* x)
{
	cout << "a\n";
	return foo_b;
}