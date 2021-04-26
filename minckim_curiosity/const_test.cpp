#include <iostream>

using std::string;
using std::cout;
using std::endl;

void	foo(char* val)
{
	(void)val;
	cout << "normal\n";
}

void	foo(const char* val)
{
	(void)val;
	cout << "const\n";
}

class	my
{
	public:
	my()
	{
		cout << "con\n";
	}
	my(const my& x)
	{
		cout << "copy\n";
	}
	~my(){}
};


my		bar()
{
	// string	a = "hello";
	// cout << (int*)&a[0] << endl;
	my	a;
	return a;
}

int		main()
{
	// const char		const_val[] = "abcd";
	// char			val[] = "abcd";

	// char			c0 = 'a';
	// const char		c1 = 'a';

	// foo(&c0);
	// foo(&c1);


	const my	a = bar();
	// cout << (int*)&a[0] << endl;
}