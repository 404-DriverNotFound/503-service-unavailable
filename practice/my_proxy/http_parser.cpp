/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minckim <minckim@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 16:46:09 by minckim           #+#    #+#             */
/*   Updated: 2021/03/22 00:03:11 by minckim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <deque>
#include <unistd.h>

enum	e_values
{
	unknown,

	method_GET,
	method_POST,
	method_HEAD,

	scheme_HTTP,
	scheme_FTP,

	header_accept_charsets,
	header_accept_language,
	header_allow,
	header_authorization,
	header_content_language,
	header_content_length,
	header_content_location,
	header_content_type,
	header_date,
	header_host,
	header_last_Modified,
	header_location,
	header_referer,
	header_retry_After,
	header_server,
	header_transfer_Encoding,
	header_user_Agent,
	header_www_authenticate,
};

struct	c_http_start_line
{
	std::string			method;
	std::string			scheme;
	int					version_major;
	int					version_minor;
};

struct	c_http_header
{
	std::string		raw;
};

struct	c_http_message
{
	c_http_start_line						start_line;
	std::map<std::string, c_http_header>	header;
	std::deque<char>						entity_body;
};

#define BUFFER_SIZE		8

namespace	ft
{
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

int		get_next_line(int fd, std::string& str)
{
	static char*		buffer[_SC_OPEN_MAX];
	static char*		mark[_SC_OPEN_MAX];
	static ssize_t		len[_SC_OPEN_MAX];

	str.clear();
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
					std::cout << "buffer del " << len[fd] << '\n';
					buffer[fd] = 0;
				}
				if (len[fd] < 0)
					return -1;
				else
					return 0;
			}
			buffer[fd][len[fd]] = 0;
		}

		std::cout << *mark[fd];

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
			str.push_back(*mark[fd]);
			mark[fd]++;
		}
		if (mark[fd] >= buffer[fd] + len[fd])
		{
			mark[fd] = buffer[fd];
		}
	}
	return -1;
}

}

class	http_parser
{
	int		fd;
	char	buffer[BUFFER_SIZE + 0];

	http_parser()
	{}

	c_http_message		operator()(int fd)
	{
	}
};

int		main(void)
{
	
}
