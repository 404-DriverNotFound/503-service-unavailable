#pragma once
class	fooC;
#include "foo.hpp"
#include <iostream>
using namespace std;
class	fooC : public foo
{
	public:
		void		handle(bar& x);
};