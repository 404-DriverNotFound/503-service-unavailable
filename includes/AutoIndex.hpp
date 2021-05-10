#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <string>

using std::string;

void	make_dir_list(const char *path);
