#pragma once
// #include "ClassModule.hpp"
#include "foo.hpp"
#include <iostream>
using namespace std;
class	bar;

class	fooA : public foo
{
	public:
		foo*		handle(bar* x);
};
