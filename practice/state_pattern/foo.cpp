#include "foo.hpp"
fooA	foo::foo_a;
fooB	foo::foo_b;
fooC	foo::foo_c;

fooA&	foo::get_a(){ return foo_a; }
fooB&	foo::get_b(){ return foo_b; }
fooC&	foo::get_c(){ return foo_c; }