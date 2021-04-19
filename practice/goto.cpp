#include <iostream>
#include <unistd.h>
#include <random>
#include <cstdlib>
#include <sys/time.h>

using namespace std;

class dummy
{
	int		a[10];
	public:
	dummy(){ cout << "c"; }
	~dummy(){ cout << "r"; }
};

long	relative_time(timeval& start, timeval& end)
{
	return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

ostream&		operator<<(ostream& o, timeval& t)
{
	cout << t.tv_sec * 1000000 + t.tv_usec << " (us)";
	return o;
}

int				myrandom(long min, long max)
{
	random_device					random_device;
	mt19937						generator(random_device());
	uniform_int_distribution<int>	distribute(min, max);
	return distribute(generator);
}

#define COUNT 1000
void	foo(int i)
{
	dummy k;
	if (i == COUNT)
		throw exception();
	int a = myrandom(1, 100) + myrandom(1, 100);
	foo(++i);
}

void	bar(int i)
{
	dummy k;
	if (i == COUNT)
		return ;
	int a = myrandom(1, 100) + myrandom(1, 100);
	bar(++i);
}

int		main()
{
	timeval		start, end;

	gettimeofday(&start, 0);
	try
	{
		foo(0);
	}
	catch(const exception& e)
	{
	}
	gettimeofday(&end, 0);
	cout << relative_time(start, end) << endl;


	gettimeofday(&start, 0);
	bar(0);
	gettimeofday(&end, 0);
	cout << relative_time(start, end) << endl;
}