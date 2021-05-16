#include "../includes/Path.hpp"

Path::Path()
{}

Path::Path(string& raw, string& query)
: _path_info(raw)
{
	Tokenizer		tokenizer(raw);
	string			path_tmp = tokenizer.chr(':');
	string			query_tmp = tokenizer.rest();
	query.swap(query_tmp);
	Tokenizer		tokenizer_path(path_tmp);
	list<string>	segment_tmp = tokenizer_path.chr_list('/');
	arrange_segment(segment_tmp);
	set_extension(segment_tmp.back());
	_segments.swap(segment_tmp);
}

Path::Path(const string& ref)
{}

Path::~Path()
{}

Path&	Path::operator=(Path& ref)
{
	return *this;
}

void		Path::arrange_segment(list<string>& segments)
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

void		Path::set_extension(string& segment)
{
	size_t	idx = segment.rfind('.');
	if (idx != -1)
		_extension = &segment[idx];
}

bool		Path::is_cgi()
{
	return !_extension.empty();
}

bool		Path::is_dir()
{
	return ft::is_dir(_path_translated.c_str());
}


// int			main()
// {
// 	string	path = "/aa/bb/cc/../../dd/../../../.:123";
// 	string	query;

// 	Path	p(path, query);

// 	for (string s : p._segments)
// 		cout << s << endl;
// 	cout << query << endl;
// 	cout << p._extension << endl;
// }