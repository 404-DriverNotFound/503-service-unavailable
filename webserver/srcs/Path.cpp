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

//------------------------------------------------------------------------------

void				Path::clear()
{
	_path_info.clear();
	_query.clear();
	_segments.clear();
	_extension.clear();
	_path_translated.clear();
}

/*==============================================================================
	Setter
==============================================================================*/

void				Path::set_path(string& raw)
{
	_segments.clear();
	
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

void				Path::set_root(const string& root)
{
	_segments.front() = root;
	set_path_translated();
	set_flag();
}

//------------------------------------------------------------------------------

bool				Path::set_index_page(const set<string>& pages)
{
	string	filename = ft::find(_path_translated, pages);
	if (filename.empty())
		return false;
	_segments.push_back(filename);
	_path_translated.append(filename);
	return true;
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
	_flag = flag_not_exist;
	struct stat		s;
	if (!_extension.empty())
	{
		_flag = flag_cgi;
	}
	else if (stat(_path_translated.c_str(), &s) == 0)
	{
		if (S_ISDIR(s.st_mode))
		{
			_flag = flag_dir;
		}
		else
		{
			_flag = flag_file;
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

Path::flag			Path::get_flag() const
{
	return _flag;
}

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