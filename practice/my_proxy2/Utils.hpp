#pragma once
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <list>
#include "UtilsTemplate.hpp"
#include <dirent.h>
#include <cstdlib>

typedef u_int8_t	uint8_t;
typedef u_int16_t	uint16_t;
typedef u_int32_t	uint32_t;
typedef u_int64_t	uint64_t;

/*##############################################################################
utils
##############################################################################*/
namespace ft{

struct	Date
{
	int	year;
	int	month;
	int	day;
	int	day_name;
	int	hour;
	int	minute;
	int	second;
};
/*--------------------------------------------------------------------------
function
--------------------------------------------------------------------------*/
void*	memset(void* dest, int c, size_t size);
void*	memcpy(void *dest, const void *src, unsigned int n);
size_t	strlen(const char* str);
int		strncmp(const char* a, const char* b, size_t len);
int		strcmp(const char* a, const char* b);
char*	strchr(const char* set, int c);
void	uppercase(std::string& str);
void	lowercase(std::string& str);
bool	get_seq_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *seq, size_t	reserve_size = 1000);
int		get_set_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char *set, size_t	reserve_size = 1000);
void	get_set_token(const std::string& origin, std::list<std::string>& tokens, const char* set);
bool	get_chr_token(const std::string &origin, std::string::const_iterator &it, std::string &token, const char c, size_t reserve_size = 1000);
std::string		which(const std::string& exe, char** env);
std::string		itoa(long n);
std::string		get_last_modified(const char* path);
Date			time_convert(long time_sec);
std::string		date_to_str(const ft::Date& d);
}		// end ft
