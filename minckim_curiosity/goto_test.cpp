#include <iostream>

using std::cout;
using std::endl;

enum	stat
{
	ZERO,
	ONE,
	TWO,
	THREE
};

int		main()
{

	int status = ZERO;

	switch (status)
	{
	case ZERO:
	ZERO:
		cout << "zero" << endl;
		goto THREE;

	case ONE:
	ONE:
		cout << "one" << endl;

	case TWO:
	TWO:
		cout << "two" << endl;

	case THREE:
	THREE:
		cout << "three" << endl;
	// 	break;
	
	// default:
	// 	break;
	}
}