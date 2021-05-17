#include "../includes/Path.hpp"

/*==============================================================================
	Constructor & Destructor
==============================================================================*/

Path::Path()
{}

//------------------------------------------------------------------------------

Path::Path(string& raw)
{
	Tokenizer		tokenizer(raw);
	string			path_tmp = tokenizer.chr(':');
	string			query_tmp = tokenizer.rest();

	_query.swap(query_tmp);

	Tokenizer		tokenizer_path(path_tmp);
	list<string>	segment_tmp = tokenizer_path.chr_list('/');

	arrange_segment(segment_tmp);
	set_extension(segment_tmp.back());
	_segments.swap(segment_tmp);
	set_path_info();
}

//------------------------------------------------------------------------------

Path::Path(const string& ref)
{}

//------------------------------------------------------------------------------

Path::~Path()
{}

//------------------------------------------------------------------------------

Path&				Path::operator=(Path& ref)
{
	return *this;
}

/*==============================================================================
	Method
==============================================================================*/

void				Path::arrange_segment(list<string>& segments)
{
	list<string>::iterator it = segments.begin();
	while (it != segments.end())
	{
		if (*it == ".")
		{
			it = segments.erase(it);
			continue;
		}
		else if (*it == "..")
		{
			if (it != segments.begin())
			{
				it = segments.erase(--it);
				it = segments.erase(it);
				continue;
			}
			else
			{
				it = segments.erase(it);
				continue;
			}
		}
		++it;
	}
}

//------------------------------------------------------------------------------

void				Path::assemble_path(string& x, const list<string>& segments)
{
	for (list<string>::const_iterator it = _segments.begin() ; it != _segments.end() ; ++it)
	{
		x.append("/");
		x.append(*it);
	}
}

/*==============================================================================
	Setter
==============================================================================*/

void				Path::set_root(const string& root)
{
	_segments.front() = root;
	set_path_translated();
	set_flag();
}

//------------------------------------------------------------------------------

void				Path::set_extension(string& segment)
{
	size_t	idx = segment.rfind('.');
	if (idx != -1)
		_extension = &segment[idx];
}

//------------------------------------------------------------------------------

void				Path::set_flag()
{
	_flag = 0;
	struct stat		s;
	if (!_extension.empty())
	{
		_flag |= (1 << flag_cgi);
	}
	else if (stat(_path_translated.c_str(), &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
		{
			_flag |= (1 << flag_dir);
		}
		else
		{
			_flag |= (1 << flag_file);
		}
	}
	return;
}

//------------------------------------------------------------------------------

void				Path::set_path_info()
{
	assemble_path(_path_info, _segments);
}

//------------------------------------------------------------------------------

void				Path::set_path_translated()
{
	assemble_path(_path_translated, _segments);
}

/*==============================================================================
	Getter
==============================================================================*/

const string&		Path::get_path_info() const
{
	return _path_info;
}

//------------------------------------------------------------------------------

const string&		Path::get_path_translated() const
{
	return _path_translated;
}

//------------------------------------------------------------------------------

const list<string>&	Path::get_segments() const
{
	return _segments;
}

//------------------------------------------------------------------------------

bool				Path::is_cgi() const
{
	return _flag | (1 << flag_cgi);
}

//------------------------------------------------------------------------------

bool				Path::is_dir() const
{
	return _flag | (1 << flag_dir);
}

//------------------------------------------------------------------------------

bool				Path::is_file() const
{
	return _flag | (1 << flag_file);
}

//------------------------------------------------------------------------------

bool				Path::is_exist() const
{
	return _flag;
}

//------------------------------------------------------------------------------

// int			main()
// {
// 	string	path = "../aa/bb/cc/dd/../.././file.exe:123";

// 	Path	p(path);


// 	cout << "path info : " << p._path_info << endl;
// 	cout << "query : " << p._query << endl;
// 	cout << "segment : ";
// 	for (string& s : p._segments)
// 		cout << s << " ";
// 	cout << endl;
// 	cout << "extension: " << p._extension << endl;

// 	p.set_root("hello");

// 	cout << "segment : ";
// 	for (string& s : p._segments)
// 		cout << s << " ";
// 	cout << endl;
// 	cout << "translated: " << p._path_translated << endl;


// }