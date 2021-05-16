#include "fooC.hpp"
#include "fooA.hpp"
#include "bar.hpp"

fooC*	foo::foo_c = new fooC;

foo*		fooC::handle(bar* x)
{
	cout << "c\n";
	return foo_a;
}