#pragma once
#include <string>
#include <unistd.h>
#include <iostream>
#include <list>
#include "UtilsTemplate.hpp"
#include <dirent.h>

namespace ft{
void*	memset(void* dest, int c, size_t size);
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
}		// end ft