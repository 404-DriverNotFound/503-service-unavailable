#include "Socket.hpp"

int		main()
{
	Socket	sock(9190, INADDR_ANY);
	timeval	time;
	fd_set	o_set;
	fd_set	r_set;
	fd_set	w_set;
	fd_set	e_set;

	char	buff[10];
	int		len;

	sock.listen(5);
	
	Socket	clnt(sock.fd);

	FD_ZERO(&o_set);
	FD_SET(clnt.fd, &o_set);

	while (42)
	{
		usleep(200000);
		time.tv_sec = 5;
		time.tv_usec = 0;;
		r_set = o_set;
		w_set = o_set;
		e_set = o_set;
		select(5, &r_set, &w_set, &e_set, &time);
		cout << "r" << FD_ISSET(clnt.fd, &r_set) << endl;
		cout << "w" << FD_ISSET(clnt.fd, &w_set) << endl;
		cout << "e" << FD_ISSET(clnt.fd, &e_set) << endl;

		if (FD_ISSET(clnt.fd, &r_set))
		{
			if ((len = read(clnt.fd, buff, 5)))
			{
				buff[len] = 0;
				cout << buff << endl;
			}
			else
			{
				cout << "len == 0\n";	
			}
		}	
		else
		{
			cout << "FD_ISSET = 0\n";
		}
		cout << endl;
		cout << endl;
	}
}