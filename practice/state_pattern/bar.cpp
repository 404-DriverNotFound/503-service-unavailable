#include "bar.hpp"
#include "foo.hpp"
#include "fooA.hpp"
#include "fooB.hpp"
#include "fooC.hpp"
bar::bar(foo* x)
: state(x)
{}

void	bar::run()
{
	state = state->handle(this);
}