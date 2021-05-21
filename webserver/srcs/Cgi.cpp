#include "../includes/Cgi.hpp"
#include "../includes/Webserver.hpp"
#include <cstdio>
#include <iostream>

// #define DBG


//------------------------------------------------------------------------------

				Cgi::~Cgi()
{}

//------------------------------------------------------------------------------

				Cgi::Cgi(string& path, string& extension, int fd_in, int fd_out)
: path(path), 
extension(extension),
fd_in(fd_in),
fd_out(fd_out)
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif
}

//------------------------------------------------------------------------------

void			Cgi::start_cgi()
{
	#ifdef DBG
	cout << __func__ << endl;
	#endif

	pid = fork();

	if (pid == 0)
	{
		lseek(fd_in, 0, SEEK_SET);
		dup2(fd_in,	0);
		dup2(fd_out, 1);
		char* const*	argv = make_argv();
		
		
		if (execve(Webserver::get_cgi_bin(extension).c_str(), argv, make_meta_variable()))
		{
			std::cerr << "CGI doesn't start" << endl;
			std::cerr << "argv[0]" << argv[0] << endl;
			std::cerr << "argv[1]" << argv[1] << endl;

			perror("");
			exit(500);
			// throw 500;	// status code 500
		}
	}
	else if (pid < 0)
	{
		throw 500;
	}
}

//------------------------------------------------------------------------------

char* const*	Cgi::make_argv()
{
	char**	argv = new char*[3];

	argv[0] = const_cast<char*>(Webserver::get_cgi_bin(extension).c_str());
	argv[1] = const_cast<char*>(path.c_str());
	argv[2] = 0;
	return argv;
}

//------------------------------------------------------------------------------

char * const*	Cgi::make_meta_variable()
{
	char**	result = new char*[meta_variables.size() + 1];

	for (size_t i = 0 ; i < meta_variables.size(); ++i)
	{
		result[i] = const_cast<char*>(meta_variables[i].c_str());
	}
	result[meta_variables.size()] = 0;
	return result;
}

//------------------------------------------------------------------------------

bool			Cgi::check_exit()
{
	is_exit = waitpid(pid, &status, WNOHANG);
	if (is_exit)
	{
		return_code = (status & 0xff00) >> 8;	// WEXITSTATUS
		if (return_code)
			throw 500;
		lseek(fd_out, 0, SEEK_SET);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void			Cgi::print_meta_variable()
{
	char* const*		meta = make_meta_variable();
	char* const*		head = meta;

	while (*meta)
		cout << *meta++ << endl;
	delete[] head;
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
