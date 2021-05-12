#include "../includes/Utils.hpp"

// #define DBG

//------------------------------------------------------------------------------

void*	ft::memset(void* dest, int c, size_t size)
{
	uint8_t*	head = static_cast<uint8_t*>(dest);
	while (size--)
		*head++ = c;
	return dest;
}

//------------------------------------------------------------------------------

void*	ft::memcpy(void *dest, const void *src, unsigned int n)
{
	unsigned char	*dest_tmp;
	unsigned char	*src_tmp;

	if (!dest && !src)
		return (0);
	dest_tmp = (unsigned char*)dest;
	src_tmp = (unsigned char*)src;
	while (n--)
		*dest_tmp++ = *src_tmp++;
	return (dest);
}

//------------------------------------------------------------------------------

size_t	ft::strlen(const char* str)
{
	const char*		head = str;

	while (*str)
	{
		++str;
	}
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

void	ft::uppercase(string& str)
{
	string::iterator	end = str.end();
	for (string::iterator it = str.begin() ; it != end ; ++it)
		if (islower(*it))
			*it -= 32;
}

//------------------------------------------------------------------------------

void	ft::lowercase(string& str)
{
	string::iterator	end = str.end();
	for (string::iterator it = str.begin() ; it != end ; ++it)
		if (isupper(*it))
			*it += 32;
}

//------------------------------------------------------------------------------

bool	ft::get_seq_token(const string &origin, str_citerator &it, string &token, const char *seq, size_t reserve_size)
{
	str_citerator	end = origin.end();
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

int		ft::get_set_token(const string &origin, str_citerator &it, string &token, const char *set, size_t reserve_size)
{
	char	*p;
	str_citerator	end = origin.end();

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

bool	ft::get_chr_token(const string &origin, str_citerator &it, string &token, char c, size_t reserve_size)
{
	str_citerator	end = origin.end();

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

//------------------------------------------------------------------------------

void	ft::get_set_token(const string& origin, std::list<string>& tokens, const char* set)
{
	str_citerator			it = origin.begin();
	string							token;

	while (ft::strchr(set, *it))
		++it;
	while (ft::get_set_token(origin, it, token, set))
	{
		while (ft::strchr(set, *it))
			++it;
		tokens.push_back(string());
		tokens.back().swap(token);
	}
	tokens.push_back(string());
	tokens.back().swap(token);
}

//------------------------------------------------------------------------------

list<string>	ft::get_set_token(const string& origin, const char* set)
{
	list<string>			tokens;
	str_citerator			it = origin.begin();
	string					token;

	while (ft::strchr(set, *it))
		++it;
	while (ft::get_set_token(origin, it, token, set))
	{
		while (ft::strchr(set, *it))
			++it;
		tokens.push_back(string());
		tokens.back().swap(token);
	}
	tokens.push_back(string());
	tokens.back().swap(token);
	return tokens;
}

//------------------------------------------------------------------------------

string		ft::which(const string& exe, char** env)
{
	string		path_env;
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
	std::list<string>	path_list;
	get_set_token(path_env, path_list, ":");
	for (std::list<string>::iterator it = path_list.begin() ; it != path_list.end() ; ++it)
	{
		if (!(dir = opendir(it->c_str())))
			continue ;
		while ((ent = readdir(dir)))
		{
			if (exe == ent->d_name)
			{
				if (*--(it->end()) != '/')
				{
					closedir(dir);
					return (string(*it) + "/" + exe);
				}
				closedir(dir);
				return (it->c_str() + exe);
			}
		}
		closedir(dir);
	}
	return "";
}

string		ft::find(const string& path_dir, set<string> files)
{
	DIR*		dir;
	dirent*		ent;

	if (!(dir = opendir(path_dir.c_str())))
		return string("");
	while ((ent = readdir(dir)))
	{
		if (files.find(ent->d_name) != files.end())
		{
			string	result(ent->d_name);
			closedir(dir);
			return result;
		}
	}
	closedir(dir);
	return string("");
}


//------------------------------------------------------------------------------

string	ft::itoa(long n)
{
	char		digit[50];
	char*		cursor = digit;
	int			sign = 1;
	string	result;

	// std::cout << '<' << n << '>' << std::endl;
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

//------------------------------------------------------------------------------

string	ft::itoa_hex(long n)
{
	char		digit[50];
	char*		cursor = digit;
	string		result;

	*cursor = 0;
	do
		*cursor++ = "0123456789abcdef"[n & 0xf];
	while ((n >>= 4));
	while (*--cursor)
		result.push_back(*cursor);
	return result;
}

//------------------------------------------------------------------------------

long	ft::atoi(const string& s)
{
	string::const_iterator	it;
	int		sign;
	long	temp = 0;

	it = s.begin();
	if (*it == '-')
	{
		sign = -1;
		++it;
	}
	for (;it != s.end(); ++it)
	{
		temp *= 10;
		temp += *it - '0';
	}
	return temp;
}

//------------------------------------------------------------------------------

long	ft::atoi_hex(const string& s)
{
	string::const_iterator	it;
	long	temp = 0;

	it = s.begin();
	for (;it != s.end(); ++it)
	{
		temp *= 16;
		temp += *it - (('0' <= *it && *it <= '9') ? '0' : (('a' <= *it && *it <= 'f') ? ('a' - 10) : ('A' - 10)));
	}
	return temp;
}

//------------------------------------------------------------------------------

char*	ft::strdup(const char* src)
{
	char *temp;
	char* dest;
	dest = new char[ft::strlen(src) + 1];
	temp = dest;
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = 0;
	// cout << temp << endl;
	return temp;
}

//------------------------------------------------------------------------------

string	ft::get_last_modified(const char* path)
{
	struct stat		stat_f;
	
	stat(path, &stat_f);
	#ifdef __APPLE__
		return ft::date_to_str(ft::time_convert(stat_f.st_mtimespec.tv_sec));
	#else
		return ft::date_to_str(ft::time_convert(stat_f.st_mtim.tv_sec));
	#endif
}

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

string		ft::date_to_str(const ft::Date& d)
{
	static string	day_name[] = 
		{"Wed", "Thu", "Fri", "Sat", "Sun", "Mon", "Tue"};
	static string	month_name[] = 
		{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	string		result;
	string		tmp;

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

//------------------------------------------------------------------------------

ssize_t		ft::file_size(const char* path)
{
	struct stat		stat_f;

	if (stat(path, &stat_f) < 0)
	{
		#ifdef DBG
		cout << "wrong_path: " << path << endl;
		#endif
		return -1;
	}
	return stat_f.st_size;
}

//------------------------------------------------------------------------------

string		ft::addr_to_str(uint32_t addr)
{
	string	result;
	result += ft::itoa((addr >> 24) & 0xff);
	result += '.';
	result += ft::itoa((addr >> 16) & 0xff);
	result += '.';
	result += ft::itoa((addr >>  8) & 0xff);
	result += '.';
	result += ft::itoa((addr      ) & 0xff);
	return result;
}

//------------------------------------------------------------------------------

int			ft::count_chr(string& str, char c)
{
	string::iterator	it = str.begin();
	string::iterator	end = str.end();
	while (*it == c && it != end)
		++it;
	if (it == end)
		return -1;
	else
		return it - str.begin();
}

//------------------------------------------------------------------------------

bool		ft::is_dir(const char* path)
{
	struct stat	stat_f;
	stat(path, &stat_f);
	// return stat_f.st_mode & S_IFDIR;
	return S_ISDIR(stat_f.st_mode);
}

//------------------------------------------------------------------------------

void		ft::str_meta_key(string& str)
{
	string::iterator	it = str.begin();
	string::iterator	end = str.end();

	while (it != end)
	{
		if ('a' <= *it && *it <= 'z')
			*it -= 32;
		else if (*it == '-')
			*it = '_';
		++it;
	}
}

//------------------------------------------------------------------------------

bool		ft::rm_df(const char *path)
{
	DIR				*dir_ptr	= NULL;
	struct dirent	*file		= NULL;

	if (ft::is_dir(path))
	{
		dir_ptr = opendir(path);
		while ((file = readdir(dir_ptr)) != NULL)
		{
			string	temp_path = path;
			temp_path += '/';
			if(ft::strcmp(file->d_name, ".") == 0 || ft::strcmp(file->d_name, "..") == 0)
			{
				continue;
			}
			temp_path += file->d_name;

			#ifdef DBG
			cout << temp_path << endl;
			#endif

			if (ft::is_dir(temp_path.c_str()))
				ft::rm_df(temp_path.c_str());
			else
				unlink(temp_path.c_str());
		}
		rmdir(path);
		closedir(dir_ptr);
	}
	else
		unlink(path);
	return (true);
}

// #include <fcntl.h>

// int		main(int argc, char** argv, char** env)
// {
// 	int		fd = open("test", O_RDONLY);
// 	cout << fd << endl;
// 	close(fd);

// 	ft::rm_df("ttt");

// 	fd = open("test", O_RDONLY);
// 	cout << fd << endl;
// 	close(fd);
// }
