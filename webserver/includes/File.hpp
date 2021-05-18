#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <string>

using std::string;

/*##############################################################################
File
##############################################################################*/

class	File
{
	public:
		enum flag
		{
			o_read = O_RDONLY,
			o_create = O_CREAT | O_TRUNC | O_RDWR,
			o_append = O_CREAT | O_APPEND | O_RDWR,
		};
	/*==========================================================================
		Member
	==========================================================================*/
	private:
		int		_fd;
		string	_name;
		bool	_is_temp;

	/*==========================================================================
		Constructor & Destructor
	==========================================================================*/
	private:
		File(const File& ref);		// 복사생성자
		File&			operator=(const File& ref);
	public:
		File(flag f = o_create);		// 임시파일
		File(const string& path, flag f = o_read);	// 실제 파일
		~File();

	/*==========================================================================
		Method
	==========================================================================*/
	private:
		void			temp_name();

	/*==========================================================================
		Getter
	==========================================================================*/
	public:
		int				get_fd() const;
		const string&	get_name() const;
		bool			is_temp() const;

		void			rewind();
		void			close();
		size_t			size();

	/*==========================================================================
		Setter
	==========================================================================*/
	public:
		void			set_file(flag flag = o_create);
		void			set_file(const string& path, flag flag = o_read);
};