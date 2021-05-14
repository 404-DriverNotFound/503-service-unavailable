#include "bar.hpp"
#include "foo.hpp"
#include "fooA.hpp"
#include "fooB.hpp"
#include "fooC.hpp"
bar::bar(foo* x)
: state(x)
{}

void	bar::set_state(fooA*	foo)
{
	state = foo;
}

void	bar::set_state(fooB*	foo)
{
	state = foo;
}

void	bar::set_state(fooC*	foo)
{
	state = foo;
}

void	bar::run()
{
	state->handle(this);
}