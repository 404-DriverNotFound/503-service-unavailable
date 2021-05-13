#pragma once
class	fooB;
#include "foo.hpp"
#include <iostream>
using namespace std;
class	fooB : public foo
{
	public:
		void		handle(bar& x);
};
