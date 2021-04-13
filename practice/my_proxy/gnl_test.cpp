#include <unistd.h>
#include <fcntl.h>
#include <deque>
#include <iostream>
#define BUFFER_SIZE		4

namespace ft{

char*		strchr(char const* str, int c)
{
	while (*str)
	{
		if (*str == c)
			return const_cast<char*>(str);
		str++;
	}
	return 0;
}

int		get_next_line(int fd, std::deque<char>& dq, int byteread = -1)
{
	static char*		buffer[BUFFER_SIZE];
	static char*		mark[BUFFER_SIZE];
	static ssize_t		len[BUFFER_SIZE];
	int					i = 0;

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

		// std::cout << *mark[fd];

		if (byteread == -1)
		{
			if (*mark[fd] == '\r' && mark[fd][1] == '\n')
			{
				mark[fd] += 2;
				return 1;
			}
			else if (*mark[fd] == '\n')
			{
				mark[fd]++;
				return 1;
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
		if (ft::strchr(charset, *it))
			return ++it;
		token.push_back(*it);
		++it;
	}
	return it;
}

}

std::ostream&	operator<<(std::ostream& o, std::deque<char>& dq)
{
	std::string	str(dq.begin(), dq.end());
	o << str;
	return o;
}

// int		main(void)
// {
// 	std::deque<char>	x;
// 	int					r;
// 	int					fd = open("file", O_RDONLY);

// 	while (1)
// 	{
// 		r = ft::get_next_line(fd, x);
// 		std::cout << x << std::endl;
// 		if (r == 0)
// 			break ;
// 	}
// }
