#pragma once
#include <sys/time.h>
#include <iostream>

/*##############################################################################
Time
##############################################################################*/

struct Time : public timeval
{
	/*==========================================================================
	Method
	==========================================================================*/
	public:
				Time();
				Time(long usec);
				Time(const Time& x);
	virtual		~Time();
	Time&		operator=(const Time& x);
	Time&		operator-=(const Time& x);
	Time		operator-(const Time& x);
	long		get_time_sec() const;
	long		get_time_usec() const;
	void		set_time(long usec);
	void		set_current();
};

/*==========================================================================
Non member Method
==========================================================================*/

std::ostream&	operator<<(std::ostream& o, const Time& t);