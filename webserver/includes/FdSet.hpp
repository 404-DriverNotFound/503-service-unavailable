#pragma once
#include <sys/select.h>
#include <sys/types.h>
#include "Utils.hpp"

/*##############################################################################
FdSet
##############################################################################*/

struct FdSet
{
	/*==========================================================================
		Member
	==========================================================================*/
	public:
		fd_set		bits;

	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	public:
		FdSet();
		FdSet&		operator=(FdSet& x);
		virtual		~FdSet();

	/*==========================================================================
		Method
	==========================================================================*/
	public:
		bool		is_set(uint64_t fd) const;		// == is_set
		void		set(uint64_t fd);
		void		clr(uint64_t fd);
		void		zero();
		void		print_bit(int size) const;
};
