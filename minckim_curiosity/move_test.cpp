#include <iostream>

class	sample
{
	public:
	int*		a;

	sample(int i)
	: a(new int(i))
	{
		std::cout << a << "  constructor" << std::endl;
	}

	sample(const sample& x)
	: a(new int(*x.a))
	{
		std::cout << a << "  copy constructor" << std::endl;
	}

	~sample()
	{
		if (a)
			delete a;
	}

	sample&		operator=(const sample& x)
	{
		if (&x == this)
			return *this;
		delete a;
		a = new int(*x.a);
		std::cout << a << "  assign" << std::endl;
		return *this;
	}

	sample&		operator=(sample&& x)
	{
		if (&x == this)
			return *this;
		delete a;
		a = x.a;
		x.a = 0;
		std::cout << a << "  move allocator" << std::endl;
		return *this;		
	}

	sample(sample&& x)
	: a(x.a)
	{
		x.a = 0;
		std::cout << "  move constructor" << std::endl;
	}
};

sample		foo()
{
	sample	s(1);

	std::cout << s.a << "  foo" << std::endl;
	return s;
}

sample		bar(sample&& x)
{
	sample&	s = x;
	std::cout << *s.a << std::endl;
	return s;
}

int		main()
{
	{
		sample s = foo();
		std::cout << s.a << std::endl;	
	}
	std::cout << std::endl;
	{
		sample	s(1);
		s = foo();		//move allocator
	}
	std::cout << std::endl;
	{
		sample s(foo());
	}
	std::cout << std::endl;
	{
		bar(3);
	}

}