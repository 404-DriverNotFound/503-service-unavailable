#include "FdSet.hpp"
#include <string.h>
// #ifdef __linux__
// #ifdef __unix__
// #ifdef __apple__

		FdSet::FdSet()
{}

		FdSet::~FdSet()
{}


bool	FdSet::get(uint64_t fd) const
{
	#ifdef __linux__
	#else
	# ifdef __apple__
	# endif
	#endif
	return fds_bits[fd >> 6] & (1ull << (fd & 63));
}

void	FdSet::set(uint64_t fd)
{
	#ifdef __linux__
	#else
	# ifdef __apple__
	# endif
	#endif
	fds_bits[fd >> 6] |= (1ull << (fd & 63));
}

void	FdSet::del(uint64_t fd)
{
	#ifdef __linux__
	fds_bits[fd / 64] ^= (1ull << (fd & 63));
	#else
	# ifdef __apple__
	# endif
	#endif
}

void	FdSet::zero()
{
	#ifdef __linux__
	#else
	# ifdef __apple__
	# endif
	#endif
	std::cout << sizeof(fds_bits) << std::endl;
	memset(fds_bits, 0, 128);
}

void	FdSet::print_bit() const
{
	for (int i = 0 ; i < sizeof(FdSet::fds_bits) * 8 ; ++i)
	{
		std::cout << FD_ISSET(i, this);
		// std::cout << get(i);
		if (!((i + 1) & 127))
			std::cout << std::endl;
	}
}

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


	a.set(511);
	a.print_bit();
	std::cout << std::endl;

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
