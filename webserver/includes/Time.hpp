#pragma once
#include <sys/time.h>
#include <iostream>

/*##############################################################################
Time
##############################################################################*/

struct Time : public timeval
{
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
				Time();
				Time(long usec);
				Time(Time& x);
	virtual		~Time();
	Time&		operator=(Time& x);
	Time&		operator-=(Time& x);
	Time		operator-(Time& x);
	long		get_time_sec() const;
	long		get_time_usec() const;
	void		set_time(long usec);
	void		set_current();
};

/*--------------------------------------------------------------------------
Non member Method
--------------------------------------------------------------------------*/

std::ostream&	operator<<(std::ostream& o, const Time& t);