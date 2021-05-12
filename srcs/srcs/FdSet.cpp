#include "../includes/FdSet.hpp"
#include <string.h>

// #define DBG

		FdSet::FdSet()
{
	zero();
}

		FdSet::~FdSet()
{}


bool	FdSet::is_set(uint64_t fd) const
{
	#ifdef __APPLE__
	return bits.fds_bits[fd >> 5] & (1u << (fd & 31));
	#else
	# ifdef __linux__
	return bits.fds_bits[fd >> 6] & (1ull << (fd & 63));
	# endif
	#endif
}

void	FdSet::set(uint64_t fd)
{
	#ifdef __APPLE__
	bits.fds_bits[fd >> 5] |= (1u << (fd & 31));
	#else
	# ifdef __linux__
	bits.fds_bits[fd >> 6] |= (1ull << (fd & 63));
	# endif
	#endif
}

void	FdSet::clr(uint64_t fd)
{
	#ifdef __APPLE__
	bits.fds_bits[fd >> 5] ^= (1ull << (fd & 31));
	#else
	# ifdef __linux__
	bits.fds_bits[fd >> 6] ^= (1ull << (fd & 63));
	# endif
	#endif
}

void	FdSet::zero()
{
	ft::memset(&bits, 0, sizeof(fd_set));
}

FdSet&	FdSet::operator=(FdSet& x)
{
	if (&x == this)
		return *this;
	bits = x.bits;
	return *this;
}

void	FdSet::print_bit(int size = 1024) const
{
	for (int i = 0 ; i < size ; ++i)
	{
		std::cout << is_set(i);
		if (!((i + 1) & 127))
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

/*
int		main(void)
{
	FdSet		a;

	a.print_bit();
	std::cout << std::endl;

	a.zero();
	a.print_bit();
	std::cout << std::endl;

	a.set(5);
	a.print_bit();
	std::cout << std::endl;

	std::cout << "set511\n";
	a.set(511);
	a.print_bit();
	std::cout << std::endl;

	std::cout << "set512 ~ 514\n";
	a.set(512);
	a.set(513);
	a.set(514);
	a.print_bit();
	std::cout << std::endl;


	std::cout << "510 " << a.get(510) << std::endl;
	std::cout << "511 " << a.get(511) << std::endl;
	std::cout << "512 " << a.get(512) << std::endl;
	std::cout << "513 " << a.get(513) << std::endl;
	std::cout << "514 " << a.get(514) << std::endl;
	std::cout << "515 " << a.get(515) << std::endl;
}
*/