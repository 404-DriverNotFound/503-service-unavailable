#include <iostream>

using namespace std;

class bar;
class fooA;
class fooB;
class fooC;

class	foo
{
	protected:
		static fooA foo_a;
		static fooB foo_b;
		static fooC foo_c;

	public:
		static fooA&	get_a(){ return foo_a; }
		static fooB&	get_b(){ return foo_b; }
		static fooC&	get_c(){ return foo_c; }
		virtual void	handle(bar& x) = 0;
};

class	fooA : public foo
{
	public:
		void		handle(bar& x)
		{
			cout << "a\n";
			x.set_state(this->foo_b);
		}
};

class	fooB : public foo
{
	public:
		void		handle(bar& x)
		{
			cout << "b\n";
			x.set_state(this->foo_c);
		}
};

class	fooC : public foo
{
	public:
		void		handle(bar& x)
		{
			cout << "c\n";
			x.set_state(this->foo_a);
		}
	
};

class	bar
{
	foo&	state;

	public:
		bar(foo& x)
		: state(x)
		{}

		void	set_state(fooA&	foo)
		{
			state = foo;
		}

		void	set_state(fooB&	foo)
		{
			state = foo;
		}

		void	set_state(fooC&	foo)
		{
			state = foo;
		}

		void	run()
		{
			state.handle(*this);
		}

};

fooA	foo::foo_a;
fooB	foo::foo_b;
fooC	foo::foo_c;

int		main()
{
	bar		c(foo::get_a());

	c.run();
	c.run();
	c.run();
	c.run();
}