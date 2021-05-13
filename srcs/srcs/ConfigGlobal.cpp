#pragma once
#include <string>
#include "../includes/ConfigGlobal.hpp"

ConfigGlobal::ConfigGlobal(int argc, char** argv, char** env)
{
	int		fd;

	if (argc == 1)
	{
		fd = open("server_config", O_RDONLY);
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
	}
	if (fd < 0)
		throw FileOpenError();
	Stream	stream(10000, fd);
	
	while (stream.fill(10000));
	

	string	line;
	stream.get_line(line);
}



ConfigGlobal::~ConfigGlobal()
{}


const char*		ConfigGlobal::FileOpenError::what() const throw()
{
	return "config file open error";
}