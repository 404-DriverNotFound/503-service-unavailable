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

bool	ft::get_seq_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *seq, size_t	reserve_size)
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

int		ft::get_set_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *set, size_t	reserve_size)
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


#include "ConfigWebserver.hpp"
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

int		main(int argc, char** argv, char** env)
{
	std::cout << ft::which("python", env) << std::endl;
	std::cout << ft::which("python3", env) << std::endl;
}