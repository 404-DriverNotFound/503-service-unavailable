#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <exception>
#include <netdb.h>
#include <fcntl.h>
#include <deque>
#define BUFFER_SIZE		4

using std::string;

enum	gnl_node
{
	GNL_READ_LINE,
	GNL_READ_BYTE
};


int								get_next_line(int fd, std::deque<char>& dq, int byteread = -1);
std::deque<char>::iterator		get_next_token(std::deque<char> str, std::deque<char>::iterator it, std::string& token, char* charset);

std::ostream&	operator<<(std::ostream& o, std::deque<char>& dq)
{
	std::string	str(dq.begin(), dq.end());
	o << str;
	return o;
}


class	client
{
private:
	sockaddr_in		addr;
	int				fd;

public:
	client()
	: fd(-1)
	{}

	client(const string& str_ip, const string& str_port)
	{
		init(str_ip, str_port);
	}

	client(const client& x)
	{
		if (fd == -1)
		{
			addr = x.addr;
			fd = x.fd;
		}
		else
		{
			close(fd);
			addr = x.addr;
			fd = dup(x.fd);
		}
	}

	client&		operator=(const client & x)
	{
		if (this == &x)
			return *this;
		if (fd == -1)
		{
			addr = x.addr;
			fd = x.fd;
		}
		else
		{
			close(fd);
			addr = x.addr;
			fd = dup(x.fd);
		}
		return *this;
	}

	void	init(const string& str_ip, const string& str_port)
	{
		if (fd != -1)
			close(fd);
		fd = socket(PF_INET, SOCK_STREAM, 0);
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(str_ip.c_str());
		addr.sin_port = htons(atoi(str_port.c_str()));
	}

	void	init(const string& domain_name)
	{
		hostent*	host = gethostbyname(domain_name.c_str());
		string		addr_tmp = inet_ntoa(**reinterpret_cast<in_addr**>(host->h_addr_list));
		if (fd != -1)
			close(fd);
		fd = socket(PF_INET, SOCK_STREAM, 0);
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(addr_tmp.c_str());
		addr.sin_port = htons(80);
	}

	string	recv_msg(int fd, std::deque<char>& byte)
	{
		int		len = 0;
		int		r;
		int		buffer_size = 10;
		char	buffer[buffer_size];
		std::deque<char>	dq;
		string	msg;

		while ((r = get_next_line(fd, dq)))
		{
			if (r < 0)
				throw read_exception();
			std::string	tmp(dq.begin(), dq.end());
			if (tmp == "\n" || tmp == "\r\n")
				break;
			std::deque<char>::iterator it = dq.begin();
			string		header;
			string		val;
			it = get_next_token(dq, it, header, ": ");
			it = get_next_token(dq, it, val, ": ");

			for (string::iterator it = header.begin() ; it != header.end() ; ++it)
			{
				if ('a' <= *it && *it <= 'z')
					*it -= 32;
			}
			if (header == "content_length")
				len = atoi(val.c_str());
			msg.insert(msg.end(), dq.begin(), dq.end());
		}
		if (len != 0)
		{
			while ((r = get_next_line(fd, dq, len)))
			{
				byte.insert(byte.end(), dq.begin(), dq.end());
			}
		}
		return msg;
	}

	void	close_server()
	{
		close(fd);
	}

	void	send_msg(int fd, const string& msg)
	{
		write(fd, msg.c_str(), msg.length());
	}

	void	connect_server()
	{
		if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
			throw connect_exception();
	}

	class connect_exception: public std::exception
	{
		virtual const char * what() const throw() { return "connect failed"; }
	};
	class read_exception: public std::exception
	{
		virtual const char * what() const throw() { return "read failed"; }
	};
	class write_exception: public std::exception
	{
		virtual const char * what() const throw() { return "write failed"; }
	};
};

class	server
{
private:
	sockaddr_in		addr;
	int				fd;

public:
	server()
	: fd(-1)
	{}

	server(const string& str_port)
	{
		init(str_port);
	}

	server(const server& x)
	{
		if (fd == -1)
		{
			addr = x.addr;
			fd = x.fd;
		}
		else
		{
			close(fd);
			addr = x.addr;
			fd = dup(x.fd);
		}
	}

	server&		operator=(const server & x)
	{
		if (this == &x)
			return *this;
		if (fd == -1)
		{
			addr = x.addr;
			fd = x.fd;
		}
		else
		{
			close(fd);
			addr = x.addr;
			fd = dup(x.fd);
		}
		return *this;
	}

	void	init(const string& str_port)
	{
		if (fd != -1)
			close(fd);
		fd = socket(PF_INET, SOCK_STREAM, 0);
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(atoi(str_port.c_str()));
		if (bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
			throw bind_exception();
	}

	void	start_listen(int n)
	{
		if (listen(fd, n) == -1)
			throw listen_exception();
	}
	
	int		accept_client()
	{
		int				result;
		socklen_t		clnt_addr_size;
		sockaddr_in		clnt_addr;
		if ((result = accept(fd, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size)) == -1)
			throw accept_exception();
		return result;
	}

	string	recv_msg(int fd, std::deque<char>& byte)
	{
		int		len = 0;
		int		r;
		int		buffer_size = 10;
		char	buffer[buffer_size];
		std::deque<char>	dq;
		string	msg;

		// int		op = fcntl(fd, F_GETFL, 0);
		// fcntl(fd, F_SETFL, op | O_NONBLOCK);

		while ((r = get_next_line(fd, dq)))
		{
			if (r < 0)
				throw read_exception();
			std::string	tmp(dq.begin(), dq.end());

			std::cout << dq << std::endl;

			// std::cout << "=>";
			// for (std::deque<char>::iterator it = dq.begin(); it != dq.end() ; ++it)
			// {
			// 	std::cout << std::setbase(16) << static_cast<int>(*it) << " ";
			// }
			// std::cout << '\n';

			std::deque<char>::iterator it = dq.begin();
			string		header;
			string		val;
			it = get_next_token(dq, it, header, ": ");
			it = get_next_token(dq, it, val, ": ");

			for (string::iterator it = header.begin() ; it != header.end() ; ++it)
			{
				if ('a' <= *it && *it <= 'z')
					*it -= 32;
			}
			if (header == "content_length")
				len = atoi(val.c_str());
			msg.insert(msg.end(), dq.begin(), dq.end());
			msg.push_back('\r');
			msg.push_back('\n');
		}
		std::cout << "---------CONTENTEND----------\n";
		if (len != 0)
		{
			while ((r = get_next_line(fd, dq, len)))
			{
				byte.insert(byte.end(), dq.begin(), dq.end());
			}
		}
		std::cout << "len : " << len << std::endl;
		return msg;
	}

	void	send_msg(int fd, const string& msg)
	{
		write(fd, msg.c_str(), msg.length());
	}

	void	close_server()
	{
		close(fd);
	}

	class bind_exception: public std::exception
	{
		virtual const char * what() const throw() { return "bind failed"; }
	};
	class listen_exception: public std::exception
	{
		virtual const char * what() const throw() { return "listen failed"; }
	};
	class accept_exception: public std::exception
	{
		virtual const char * what() const throw() { return "accept failed"; }
	};
	class read_exception: public std::exception
	{
		virtual const char * what() const throw() { return "read failed"; }
	};
	class write_exception: public std::exception
	{
		virtual const char * what() const throw() { return "write failed"; }
	};
};

string	extract_url(string str)
{
	string		url;

	for (string::iterator c = str.begin() ; c != str.end() ; ++c)
		if ('A' <= *c && *c <= 'Z')
			*c += 32;
	int idx = str.find("host");
	
	idx += 6;
	string::iterator it = str.begin() + idx;
	while (*it != ' ' && *it != ':')
	{
		url.push_back(*it);
		++it;
	}

	std::cout << "!!URL!!: " << url << std::endl;
	return url;
}

// int		main(int argc, char **argv)
// {


// 	if (argc != 2)
// 	{
// 		std::cout << "wrong argument\n";
// 		exit(1);
// 	}

// 	try
// 	{
// 		// while (1)
// 		{
// 			server		proxy;
// 			proxy.init(argv[1]);
// 			proxy.start_listen(5);
// 			int		browser = proxy.accept_client();
// 			string	msg;
// 			msg = proxy.recv_msg(browser);
// 			std::cout << "msg from client:\n" << msg << std::endl;
// 			msg = "HTTP/1.1 200 OK\r\nContent-length: 10\r\n\r\nHelloWorld";
// 			proxy.send_msg(browser, msg);
// 			usleep(1000000);
// 			close(browser);
// 			usleep(1000000);
// 			proxy.close_server();
// 		}

// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	return 0;
// }

int		get_next_line(int fd, std::deque<char>& dq, int byteread)
{
	static char*		buffer[BUFFER_SIZE];
	static char*		mark[BUFFER_SIZE];
	static ssize_t		len[BUFFER_SIZE];
	int					i = 0;

	// std::cout << "call gnl: ";
	dq.clear();
	if (!buffer[fd])
	{
		buffer[fd] = new char[BUFFER_SIZE];
		mark[fd] = buffer[fd];
	}
	while (42)
	{
		if (mark[fd] == buffer[fd])
		{
			len[fd] = read(fd, buffer[fd], BUFFER_SIZE);
			if (len[fd] <= 0)
			{
				if (buffer[fd])
				{
					delete[] buffer[fd];
					// std::cout << "buffer del " << len[fd] << '\n';
					buffer[fd] = 0;
				}
				if (len[fd] < 0)
					return -1;
				else
					return 0;
			}
			buffer[fd][len[fd]] = 0;
		}

		// std::cout << "[" << std::setbase(16) << static_cast<int>(*mark[fd]) << "] ";

		if (byteread == -1)
		{
			if (*mark[fd] == '\r' && mark[fd][1] == '\n')
			{
				mark[fd] += 2;
				return i;
			}
			else if (*mark[fd] == '\n' || *mark[fd] == '\r')
			{
				mark[fd]++;
				return i;
			}
			else
			{
				dq.push_back(*mark[fd]);
				mark[fd]++;
				i++;
			}
		}
		else
		{
			if (byteread == 0)
				return i;
			byteread--;
			dq.push_back(*mark[fd]);
			mark[fd]++;
			i++;
		}
		if (mark[fd] >= buffer[fd] + len[fd])
		{
			mark[fd] = buffer[fd];
		}
	}
	return -1;
}

std::deque<char>::iterator		get_next_token(std::deque<char> str, std::deque<char>::iterator it, std::string& token, char* charset)
{
	while (42)
	{
		if (it == str.end())
			return it;
		if (strchr(charset, *it))
			return ++it;
		token.push_back(*it);
		++it;
	}
	return it;
}





int		main(int argc, char **argv)
{


	if (argc != 2)
	{
		std::cout << "wrong argument\n";
		exit(1);
	}

	try
	{
		server		proxy;
		proxy.init(argv[1]);
		proxy.start_listen(5);
		while (1)
		{
			int		browser = proxy.accept_client();

			client				dest;
			string				msg;
			std::deque<char>	d;
			std::deque<char>	d2;

			msg = proxy.recv_msg(browser, d);
			proxy.close_server();
			std::cout << "-------------------------------\n";
			usleep(1000000);
			// std::cout << "msg from browser:\n" << msg << std::endl;
			// string url = extract_url(msg);
			// dest.init(url);
			// dest.connect_server();
			// dest.send_msg(browser, msg);
			// msg = dest.recv_msg(browser, d2);
			// std::cout << "msg from server:\n" << msg << std::endl;
			// proxy.send_msg(browser, msg);
			// dest.close_server();
			// proxy.close_server();
		}
		// close(browser);

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}