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
			Cgi();
	void	init(char* path, char** meta_variable);
	void	start_cgi();
	void	connect();
	void	disconnect();
	void	destroy_pipe();
};