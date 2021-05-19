#pragma once
#include <string>
#include <map>
#include "Stream.hpp"
#include "File.hpp"

using std::string;
using std::map;
/*##############################################################################
	
	Http

##############################################################################*/

class Http
{
	/*==========================================================================
		Member
	==========================================================================*/
	protected:
		map<string, string>		_headers;
		string					_protocol;
		string					_line;
		Stream					_stream;
		File*					_file;
		ssize_t					_content_length;
		string					_scheme;
	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		Http(const Http&);
		Http&	operator=(const Http&);
		Http();
	public:
		Http(int fd);
		~Http();
	/*==========================================================================
		Method
	==========================================================================*/
		bool			get_next_line();
		void			clear();
	/*==========================================================================
		Setter
	==========================================================================*/
		void			set_start_line(string& line);
		string			get_start_line(int code);
		virtual void	set_file(File::flag flag = File::o_create) = 0;
		virtual void	set_file(const string& path, File::flag f = File::o_read) = 0;
		void			del_file();
		void			set_header(string& line);
		virtual void	set_stream_fd(int fd) = 0;
		virtual void	set_stream_file_fd() = 0;
	/*==========================================================================
		Getter
	==========================================================================*/
		map<string, string>&	get_headers();
		const string&			get_header(const string& key);
		Stream&					get_stream();
		string&					get_string();
		int						get_file_fd();
		const string&			get_file_name();
		size_t					get_file_size();
};
