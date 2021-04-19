#include "Utils.hpp"

//------------------------------------------------------------------------------

void*	memset(void* dest, int c, size_t size)
{
	uint8_t*	head = static_cast<uint8_t*>(dest);
	while (size--)
		*head++ = c;
	return dest;
}

//------------------------------------------------------------------------------

size_t	ft::strlen(const char* str)
{
	const char*		head = str;

	while (*str++)
	{}
	return str - head;
}

//------------------------------------------------------------------------------

int		ft::strncmp(const char* a, const char* b, size_t len)
{
	while (len-- && *a && *b)
	{
		if (*a != *b)
			return *a - *b;
		a++;
		b++;
	}
	return 0;
}

//------------------------------------------------------------------------------

int		ft::strcmp(const char* a, const char* b)
{
	while (*a && *b)
	{
		if (*a != *b)
			return *a - *b;
		a++;
		b++;
	}
	return 0;
}

//------------------------------------------------------------------------------

char*	ft::strchr(const char* set, int c)
{
	while (*set)
	{
		if (*set == c)
			return (char*)set;
		set++;
	}
	return 0;
}

//------------------------------------------------------------------------------

void	ft::uppercase(std::string& str)
{
	std::string::iterator	end = str.end();
	for (std::string::iterator it = str.begin() ; it != end ; ++it)
		if (islower(*it))
			*it -= 32;
}

//------------------------------------------------------------------------------

void	ft::lowercase(std::string& str)
{
	std::string::iterator	end = str.end();
	for (std::string::iterator it = str.begin() ; it != end ; ++it)
		if (isupper(*it))
			*it += 32;
}

//------------------------------------------------------------------------------

bool	ft::get_seq_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *seq, size_t reserve_size)
{
	std::string::const_iterator	end = origin.end();
	int						seq_len;

	seq_len = ft::strlen(seq);
	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if (!ft::strncmp(it.base(), seq, seq_len))
		{
			it += seq_len;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}

//------------------------------------------------------------------------------

int		ft::get_set_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *set, size_t reserve_size)
{
	char	*p;
	std::string::const_iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while (it != end)
	{
		if ((p = ft::strchr(set, *it)))
		{
			it += 1;
			return *p;
		}
		token.push_back(*it);
		++it;
	}
	return 0;
}

//------------------------------------------------------------------------------

bool	ft::get_chr_token(const std::string &origin, std::string::const_iterator &it, std::string &token, char c, size_t reserve_size)
{
	std::string::const_iterator	end = origin.end();

	token.clear();
	token.reserve(reserve_size);
	while(it != end)
	{
		if (*it == c)
		{
			++it;
			while (it != end && *it == c)
				++it;
			return true;
		}
		token.push_back(*it);
		++it;
	}
	return false;
}

void	ft::get_set_token(const std::string& origin, std::list<std::string>& tokens, const char* set)
{
	std::string::const_iterator			it = origin.begin();
	std::string							token;

	while (ft::strchr(set, *it))
		++it;
	while (ft::get_set_token(origin, it, token, set))
	{
		while (ft::strchr(set, *it))
			++it;
		tokens.push_back(std::string());
		tokens.back().swap(token);
	}
	tokens.push_back(std::string());
	tokens.back().swap(token);
}

std::string		ft::which(const std::string& exe, char** env)
{
	std::string		path_env;
	dirent*			ent;
	DIR*			dir;

	while (*env)
	{
		if (ft::strncmp("PATH", *env, 4) == 0)
		{
			path_env.assign(*env + 5, *env + ft::strlen(*env));
			break ;
		}
		++env;
	}
	std::list<std::string>	path_list;
	get_set_token(path_env, path_list, ":");
	for (std::list<std::string>::iterator it = path_list.begin() ; it != path_list.end() ; ++it)
	{
		if (!(dir = opendir(it->c_str())))
			continue ;
		while ((ent = readdir(dir)))
		{
			if (exe == ent->d_name)
			{
				if (it->back() != '/')
					return (std::string(*it) + "/" + exe);
				return (it->c_str() + exe);
			}
		}
	}
	return "";
}

std::string	ft::itoa(long n)
{
	char		digit[50];
	char*		cursor = digit;
	int			sign = 1;
	std::string	result;

	std::cout << '<' << n << '>' << std::endl;
	*cursor = 0;
	if (static_cast<unsigned long>(n) == 0x8000000000000000l)
		return "-9223372036854775808";
	if (n < 0)
	{
		sign = -1;
		n = -n;
	}
	do
		*cursor++ = n % 10 + '0';
	while ((n /= 10));
	if (sign < 0)
		*cursor++ = '-';
	while (*--cursor)
		result.push_back(*cursor);
	return result;
}


std::string	ft::get_last_modified(const char* path)
{
	struct stat		stat_f;
	
	stat(path, &stat_f);
	#ifdef __APPLE__
		return ft::date_to_str(ft::time_convert(stat_f.st_mtimespec.tv_sec));
	#else
		return ft::date_to_str(ft::time_convert(stat_f.st_mtim.tv_sec));
	#endif
}

ft::Date		ft::time_convert(long time_sec)
{
	static int	days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int			is_skip = 0;
	Date		d;

	d.second = time_sec % 60;
	d.minute = time_sec % 3600 / 60;
	d.hour = time_sec % 86400 / 3600;
	
	int day = time_sec / 86400;

	d.day_name = day % 7; // 0 == wed
	d.year = 1970;
	d.month = 1;
	d.day = 1;

	while (42)
	{
		is_skip = 0;
		if (d.year % 4 == 0)
			is_skip = 1;
		if (d.year % 100 == 0)
			is_skip = 0;
		if (d.year % 400 == 0)
			is_skip = 1;
		day -= 365 + is_skip;
		if (day <= 0)
		{
			day += 365 + is_skip;
			break;
		}
		d.year++;
	}

	days[1] = 28 + is_skip;
	for (int i = 0 ; i < 12 ; ++i)
	{
		day -= days[i];
		if (day <= 0)
		{
			day += days[i];
			break;
		}
		d.month++;
	}
	d.day += day;
	return d;
}

std::string		ft::date_to_str(const ft::Date& d)
{
	static std::string	day_name[] = 
		{"Wed", "Thu", "Fri", "Sat", "Sun", "Mon", "Tue"};
	static std::string	month_name[] = 
		{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	std::string		result;
	std::string		tmp;

	result.append(day_name[d.day_name]);
	result.append(", ");
	tmp = ft::itoa(d.day);
	result.append(tmp.length() == 1 ? "0" + tmp : tmp);
	result.append(" ");
	result.append(month_name[d.month - 1]);
	result.append(" ");
	result.append(ft::itoa(d.year));
	result.append(" ");
	tmp = ft::itoa(d.hour);
	result.append(tmp.length() == 1 ? "0" + tmp : tmp);
	result.append(":");
	tmp = ft::itoa(d.minute);
	result.append(tmp.length() == 1 ? "0" + tmp : tmp);
	result.append(":");
	tmp = ft::itoa(d.second);
	result.append(tmp.length() == 1 ? "0" + tmp : tmp);
	result.append(" GMT");
	return result;
}

// int		main(int argc, char** argv, char** env)
// {
// 	timeval		t;
// 	gettimeofday(&t, 0);
// 	std::cout << ft::date_to_str(ft::time_convert(t.tv_sec)) << std::endl;
// }
