#include "Cgi.hpp"
// #include <cstdio>
#include <iostream>

Cgi::map_path	Cgi::cgi_bin;

//------------------------------------------------------------------------------

				Cgi::Cgi()
: fd_in(fd_read[0]), fd_out(fd_write[1]), path(0), meta_variable(0)
{}

//------------------------------------------------------------------------------

void			Cgi::init(const char* path, char** meta_variable)
{
	this->path = path;
	this->meta_variable = meta_variable;
	set_extension();
	pipe(fd_write);
	pipe(fd_read);
}

//------------------------------------------------------------------------------

void			Cgi::start_cgi()
{
	pid = fork();

	if (pid == 0)
	{
		dup2(fd_write[0], 0);
		dup2(fd_read[1], 1);
		close(fd_write[1]);
		close(fd_read[0]);
		if (execve(cgi_bin[extension].c_str(), make_argv(), meta_variable))
			throw 500;	// status code 500
	}
	else
	{
		close(fd_read[1]);
		close(fd_write[0]);
	}
}

//------------------------------------------------------------------------------

void			Cgi::destroy_pipe()
{
	close(fd_read[0]);
	close(fd_write[1]);
}

//------------------------------------------------------------------------------

void			Cgi::set_extension()
{
	extension.assign(path.begin() + path.find_last_of("."), path.end());
}

//------------------------------------------------------------------------------

char* const*	Cgi::make_argv()
{
	char**	argv = new char*[3];

	argv[0] = const_cast<char*>(cgi_bin[extension].c_str());
	argv[1] = const_cast<char*>(path.c_str());
	argv[2] = 0;
	return argv;
}

void			Cgi::check_exit()
{
	is_exit = waitpid(pid, &status, WNOHANG);
	if (is_exit)
		return_code = (status & 0xff00) >> 8;	// WEXITSTATUS
}

void			Cgi::terminate()
{
	destroy_pipe();
}

/*
int			main(void)
{
	Cgi		c0("exe0", 0);
	// Cgi		c1("./exe1");
	c0.start_cgi();
	c0.connect();
	// printf("this must be in file0\n");
	// std::cout << "this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0this must be in file0";
	// std::cout << "this must be in file0" << std::endl;
	// write(1, "this must be in file0\n", strlen("this must be in file0\n"));
	// for (int i = 0 ; i < 80000 ;)
	// {
	// 	write(1, "0123456789abcdef\n", 17);
	// 	i += 11;
	// 	// usleep(100000);
	// }
	char*	str = new char[7000];
	memset(str, 'a', 7000);
	write(1, str, 7000);
	write(1, "\n", 1);
	c0.disconnect();
	printf("this must be in test0\n");
	
	
	c0.connect();
	memset(str, 'a', 7000);
	write(1, str, 7000);
	write(1, "\n", 1);
	c0.disconnect();
	// std::cout << "this must be in stdout" << std::endl;
	// write(1, "this must be in stdout\n", strlen("this must be in stdout\n"));
	c0.destroy_pipe();
}
*/