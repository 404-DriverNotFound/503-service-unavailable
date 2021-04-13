#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

using std::cout;
using std::endl;

int		main(int argc, char **argv)
{
	hostent*		host;

	if (argc != 2)
		return 1;

	host = gethostbyname(argv[1]);

	cout << "official name: "<< host->h_name << endl;

	for (char** str = host->h_aliases ; *str ; str++)
		cout << "alias: " << *str << endl;

	cout << "addr type: " << (host->h_addrtype == AF_INET ? "IPv4" : "IPv6") << endl;

	cout << "addr length: " << host->h_length << endl;

	if (host->h_addrtype == AF_INET)
	{
		for (in_addr** addr = reinterpret_cast<in_addr**>(host->h_addr_list) ; *addr ; addr++)
			cout << "addr: "<< inet_ntoa(**reinterpret_cast<in_addr**>(addr)) << endl;
	}

	return 0;
}