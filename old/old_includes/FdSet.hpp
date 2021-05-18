#pragma once
#include <sys/select.h>
#include <sys/types.h>
#include "Utils.hpp"

/*##############################################################################
FdSet
##############################################################################*/
struct FdSet
{
	fd_set		bits;
	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
	public:
				FdSet();
	FdSet&		operator=(FdSet& x);
	virtual		~FdSet();
	bool		get(uint64_t fd) const;		// == is_set
	void		set(uint64_t fd);
	void		del(uint64_t fd);
	void		zero();
	void		print_bit(int size) const;
};
