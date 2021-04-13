#include <iostream>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>

// -----------------------------------------------------------------------------
// relative time
timeval			relative_time(timeval& start, timeval& end)
{
	timeval		result = {end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec};
	return result;
}

// -----------------------------------------------------------------------------
// ostream << timeval
std::ostream&	operator<<(std::ostream& o, timeval t)
{
	o << (t.tv_sec * 1000000 + t.tv_usec) << " (us)";
	return o;
}

#define SIZE 30
#define COUNT 100000

void			f0(const char* s)
{
	const char*		head = s;
	char*			dest = new char[SIZE];
	while (*s)
		s++;
	s = head;
	while (*s)
		*dest++ = *s++;
}

void			f1(const char* s)
{
	const char*		head = s;
	char*			dest = new char[SIZE];
	while (*s)
		*dest++ = *s++;
}


int		main()
{
	char*		str0;
	char*		str1;


	timeval		start;
	timeval		end;


	gettimeofday(&start, 0);

	for (int i = 0 ; i < COUNT ; i++)
	{
		str0 = new char[SIZE];
		memset(str0, 1, SIZE);
		str0[SIZE - 1] = 0;
		f0(str0);
		delete[] str0;
	}

	gettimeofday(&end, 0);
	std::cout << relative_time(start, end) << std::endl;
	


	gettimeofday(&start, 0);

	for (int i = 0 ; i < COUNT ; i++)
	{
		str1 = new char[SIZE];
		memset(str1, 1, SIZE);
		str1[SIZE - 1] = 0;
		f1(str1);
		delete[] str1;
	}

	gettimeofday(&end, 0);
	std::cout << relative_time(start, end) << std::endl;

}