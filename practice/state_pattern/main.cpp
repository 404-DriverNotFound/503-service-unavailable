// #include "ClassModule.hpp"
#include "foo.hpp"
#include "bar.hpp"

int		main()
{
	bar		c(foo::get_a());

	c.run();
	c.run();
	c.run();
	c.run();
}