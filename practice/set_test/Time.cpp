#include "Time.hpp"

// #define DBG

/*------------------------------------------------------------------------------
Method
------------------------------------------------------------------------------*/
			Time::Time()
{	gettimeofday(this, 0);	}

//------------------------------------------------------------------------------

			Time::Time(long usec)
{	set_time(usec);	}

//------------------------------------------------------------------------------

			Time::Time(Time& x)
{
	tv_sec = x.tv_sec;
	tv_usec = x.tv_usec;
}

//------------------------------------------------------------------------------

			Time::~Time()
{}

//------------------------------------------------------------------------------

Time&		Time::operator=(Time& x)
{
	if (this == &x)
		return *this;
	tv_sec = x.tv_sec;
	tv_usec = x.tv_usec;
	return *this;
}

//------------------------------------------------------------------------------

Time&		Time::operator-=(Time& x)
{
	tv_sec -= x.tv_sec;
	tv_usec -= x.tv_usec;
	return *this;
}

//------------------------------------------------------------------------------

Time		Time::operator-(Time& x)
{
	Time	a(*this);
	return a -= x;
}

//------------------------------------------------------------------------------

long		Time::get_time_sec() const
{	return tv_sec;	}

//------------------------------------------------------------------------------

long		Time::get_time_usec() const
{	return tv_sec * 1000000 + tv_usec;	}

//------------------------------------------------------------------------------

void		Time::set_time(long usec)
{	
	tv_sec = usec / 1000000;
	tv_usec = usec - tv_sec * 1000000;
}

//------------------------------------------------------------------------------

void		Time::set_current()
{	
	gettimeofday(this, 0);
}

/*--------------------------------------------------------------------------
Non member Method
--------------------------------------------------------------------------*/

std::ostream&	operator<<(std::ostream& o, const Time& t)
{
	o << t.get_time_usec() << "(us)";
	return o;
}

/*
#include <unistd.h>
int		main()
{
	Time x;
	std::cout << x << std::endl;
	usleep(1000000);
	Time y;
	std::cout << y << std::endl;
	std::cout << (y - x) << std::endl;
}
*/