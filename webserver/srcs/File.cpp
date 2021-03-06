#include "../includes/File.hpp"
#include "../includes/Webserver.hpp"

#if __BONUS__ == 1
pthread_mutex_t		File::mutex_temp_name;
#endif

File::File(flag f)
: _name(Webserver::config.get_temp_dir()),
  _is_temp(true)
{
	temp_name();
	_fd = open(_name.c_str(), f, 0644);
}

File::File(const string& path, flag f)	// 경로로 열기
: _name(path),
  _is_temp(false)
{
	_fd = open(path.c_str(), f, 0644);
}

File::~File()
{
	close();
}

void			File::temp_name() 
{
	static int	serial = 0;
	_name.append("/temp");
	#if __BONUS__ == 1
	pthread_mutex_lock(&mutex_temp_name);
	#endif

	_name.append(ft::itoa(serial++));

	#if __BONUS__ == 1
	pthread_mutex_unlock(&mutex_temp_name);
	#endif
}

int				File::get_fd() const
{
	return _fd;
}

const string&	File::get_name() const
{
	return _name;
}

bool			File::is_temp() const
{
	return _is_temp;
}

void			File::rewind()
{
	lseek(_fd, 0, SEEK_SET);
}

void			File::close()
{
	if (_fd >= 0)
	{
		::close(_fd);
		if (_is_temp)
			ft::rm_df(_name.c_str());
		_name.clear();
		_fd = -1;
	}
}

void			File::set_file(flag f)
{
	close();
	_name = Webserver::config.get_temp_dir();
	_is_temp = true;
	temp_name();
	_fd = open(_name.c_str(), f, 0644);	
}

void			File::set_file(const string& path, flag f)
{
	close();
	_name = path;
	_is_temp = false;
	temp_name();
	_fd = open(path.c_str(), f, 0644);
}

size_t			File::size()
{
	struct stat		s;
	if (fstat(_fd, &s))
		return -1;
	return s.st_size;
}

void			File::mutex_init_tempname()
{
	#if __BONUS__ == 1
	pthread_mutex_init(&mutex_temp_name, 0);
	#endif
}