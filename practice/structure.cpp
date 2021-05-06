typedef unsigned short	uint32_t;
typedef short			int16_t;
typedef unsigned int	uint32_t;

#include <sys/types.h>

struct sa_family_t
{

};

struct sockaddr
{
	sa_family_t		sin_family;		// 주소 체계(Address Family)
	char			sa_data[14];	// 주소 정보
};

typedef uint32_t	in_addr_t;

struct in_addr
{
	in_addr_t		s_addr;			// 32비트 IPv4 인터넷 주소
};


struct sockaddr_in
{
	sa_family_t		sin_family;		// 주소 체계(Address Family)
	uint32_t		sin_port;		// 16비트 TCP/UDP 포트 번호
	struct in_addr	sin_addr;		// 32비트 IP주소
	char			sin_zero[8];	// 사용되지 않음
};

struct hostent
{
	char*	h_name;
	char**	h_aliases;
	int		h_addrtype;
	int		h_length;
	char**	h_addr_list;
};

struct hostent*		gethostbyname(const char* hostname);

#include <sys/select.h>
#include <sys/time.h>
int		select(
			int				maxfd, 
			fd_set*			readset, 
			fd_set*			writeset,
			fd_set*			exceptset,
			const timeval*	timeout
		);
		