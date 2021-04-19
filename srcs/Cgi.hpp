#pragma once
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include "Utils.hpp"

/*##############################################################################
Cgi
##############################################################################*/
struct Cgi
{
	/*--------------------------------------------------------------------------
	Typedef
	--------------------------------------------------------------------------*/
	public:
	typedef std::map<std::string, std::string>	 map_path;

	/*--------------------------------------------------------------------------
	Member
	--------------------------------------------------------------------------*/
	public:
	int				pid;
	int				fd_write[2];	// server ---> cgi
	int				fd_read[2];		//    cgi ---> server
	int&			fd_in;			// server's new stdin
	int&			fd_out;			// server's new stdout
	std::string		path;
	std::string		extension;
	char**			meta_variable;
	static map_path	cgi_bin;

	/*--------------------------------------------------------------------------
	Method
	--------------------------------------------------------------------------*/
					Cgi();
	void			init(char* path, char** meta_variable);
	void			start_cgi();
	void			set_extension();
	char* const*	make_argv();
	void			destroy_pipe();
};