#pragma once
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <list>
#include <set>
#include "UtilsTemplate.hpp"
#include <dirent.h>
#include <cstdlib>

typedef u_int8_t	uint8_t;
typedef u_int16_t	uint32_t;
typedef u_int32_t	uint32_t;
typedef u_int64_t	uint64_t;

/*##############################################################################
utils
##############################################################################*/

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::set;
typedef string::const_iterator str_citerator;

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
void*			memset(void* dest, int c, size_t size);
void*			memcpy(void *dest, const void *src, unsigned int n);
size_t			strlen(const char* str);
int				strncmp(const char* a, const char* b, size_t len);
int				strcmp(const char* a, const char* b);
char*			strchr(const char* set, int c);
void			uppercase(string& str);
void			lowercase(string& str);
bool			get_seq_token(const string &origin, str_citerator &it, string &token, const char *seq, size_t reserve_size = 1000);
int				get_set_token(const string &origin, str_citerator &it, string &token, const char *set, size_t reserve_size = 1000);
void			get_set_token(const string& origin, std::list<string>& tokens, const char* set);
list<string>	get_set_token(const string& origin, const char* set);
bool			get_chr_token(const string &origin, str_citerator &it, string &token, const char c, size_t reserve_size = 1000);
string			which(const string& exe, char** env);
string			find(const string& path, set<string> files);
long			atoi(const string& s);
string			itoa(long n);
string			itoa_hex(long n);
char*			strdup(char* dest, const char* src);
long			atoi_hex(const string& s);
string			get_last_modified(const char* path);
Date			time_convert(long time_sec);
string			date_to_str(const ft::Date& d);
ssize_t			file_size(const char* path);
string			addr_to_str(uint32_t addr);
int				count_chr(string& str, char c);
bool			is_dir(const char* path);
}		// end ft
