#include <unistd.h>
#include <sys/wait.h>

class Cgi
{
	int		pid;
	int		fd_pipe[2];
	int		fd_out_tmp;
	char*	path;
	char*	extention;
	char**	meta_variable;

	public:
			Cgi(char* path, char** meta_variable);
	void	start_cgi();
	void	connect();
	void	disconnect();
	void	stdout_set();
	void	stdout_restore();
	void	destroy_pipe();
};