#include <unistd.h>
#include <fcntl.h>
#include <iostream>

using std::cout;
using std::endl;


int			main(void)
{
	int		status;


	int		fd_in = open("file_in", O_RDONLY);
	int		fd_out = open("file_out", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	int		pid = fork();	
	if (fd_in < 0)
	{
		cout << "bad file: " << fd_in << endl;
		exit(1);
	}

	if (pid == 0)
	{
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		char*	argv[] = {"./cgi_tester", 0};
		char*	env[] = {
			"AUTH_TYPE=",
			"CONTENT_LENGTH=4",
			"CONTENT_TYPE=",
			"GATEWAY_INTERFACE=CGI/1.1",
			"PATH_INFO=aaaaaa",
			"PATH_TRANSLATED=./file.bla",
			"QUERY_STRING=./baba",
			"REMOTE_ADDR=",
			"REMOTE_IDENT=",
			"REMOTE_USER=",
			"REQUEST_METHOD=PUT",
			"REQUEST_URI=",
			"SCRIPT_NAME=",
			"SERVER_NAME=",
			"SERVER_PORT=",
			"SERVER_PROTOCOL=HTTP/1.1",
			"SERVER_SOFTWARE=",
			0};


		if (execve("./cgi_tester", argv, env))
		{
			cout << "cgi doesnt started\n";
			exit(1);
		}
	}
	else
	{
		wait(&status);
		cout << "end\n";
	}
}