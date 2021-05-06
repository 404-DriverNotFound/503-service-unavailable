#include "../includes/Cgi.hpp"
#include <cstdio>
#include <iostream>

Cgi::map_path	Cgi::cgi_bin;

//------------------------------------------------------------------------------

				Cgi::~Cgi()
{}

//------------------------------------------------------------------------------

				Cgi::Cgi(string& path, string& extension, 
				int fd_in, int fd_out, char** meta_variable)
: path(path), 
extension(extension),
fd_in(fd_in),
fd_out(fd_out),
meta_variable(meta_variable)
{
	cout << __func__ << endl;

}

//------------------------------------------------------------------------------

void			Cgi::start_cgi()
{
	cout << __func__ << endl;


	pid = fork();

	if (pid == 0)
	{
		lseek(fd_in, 0, SEEK_SET);
		dup2(fd_in,	0);
		dup2(fd_out, 1);
		char* const*	argv = make_argv();
		
		
		if (execve(cgi_bin[extension].c_str(), argv, meta_variable))
		{
			std::cerr << "CGI doesn't start" << endl;
			std::cerr << "argv[0]" << argv[0] << endl;
			std::cerr << "argv[1]" << argv[1] << endl;



			perror("");
			throw 500;	// status code 500
		}
		exit(1);
	}
	else if (pid < 0)
	{
		throw 500;
	}
	delete[] meta_variable;
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

bool			Cgi::check_exit()
{
	is_exit = waitpid(pid, &status, WNOHANG);
	if (is_exit)
	{
		return_code = (status & 0xff00) >> 8;	// WEXITSTATUS
		// lseek(fd_out, 0, SEEK_SET);
		return true;
	}
	return false;
}

void			Cgi::set_path_cgi_bin(char** env)
{
	Cgi::cgi_bin[".php"] = ft::which("php", env);
	Cgi::cgi_bin[".py"] = ft::which("python3", env);
	Cgi::cgi_bin[".bla"] = "./test/cgi_tester";
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
