#pragma once
class	fooA;
#include "foo.hpp"
#include <iostream>
using namespace std;
class	fooA : public foo
{
	public:
		void		handle(bar& x);
};
