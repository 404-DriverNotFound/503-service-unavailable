#include "classes.hpp"

int		main()
{
	classes::bar		c(classes::foo::get_a());

	c.run();
	c.run();
	c.run();
	c.run();
}