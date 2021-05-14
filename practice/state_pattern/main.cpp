#include "ClassModule.hpp"

int		main()
{
	bar		c(foo::get_a());

	c.run();
	c.run();
	c.run();
	c.run();
}