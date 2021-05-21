#include "../includes/Tokenizer.hpp"
#include <iostream>
#include <iomanip>
using std::string;
using std::cout;
using std::endl;

Tokenizer::Tokenizer()
{}

Tokenizer::Tokenizer(string& str_input)
{
	str.swap(str_input);
	it = str.begin().base();
	end = str.end().base();
}

Tokenizer::Tokenizer(const string& str)
: str(str), it(str.begin().base()), end(str.end().base())
{}

Tokenizer::Tokenizer(const Tokenizer& ref)
: str(ref.str), it(str.begin().base()), end(str.end().base())
{}

Tokenizer::~Tokenizer()
{}

Tokenizer&	Tokenizer::operator=(const Tokenizer& ref)
{
	if (this == &ref)
		return *this;
	str = ref.str;
	it = str.begin().base();
	end = str.end().base();
	return *this;
}
// 최상의 성능을 내려면 문자열을 새로 선언할 것.
// ex) string token = tokenizer.chr('s');

string		Tokenizer::rest()
{
	return string(it, end);
}

string		Tokenizer::chr(char sep)
{
	const char*		start = it;

	while (it != end && *it != sep)
	{
		++it;
	}
	string	token(start, it);
	while (*it == sep)
	{
		++it;
	}
	return token;
}

list<string>	Tokenizer::chr_list(char sep)
{
	list<string>	lines;
	
	while (*it == sep)
	{
		++it;
	}
	while (it != end)
	{
		string line = chr(sep);
		lines.push_back(string());
		lines.back().swap(line);
	}
	return lines;
}

list<string>	Tokenizer::path_segment()
{
	list<string>	lines;
	
	while (it != end)
	{
		if (*it == '/')
		{
			lines.push_back(string("/"));
			++it;
			while (it != end && *it != '/')
			{
				lines.back().push_back(*it);
				++it;
			}
		}
	}

	return lines;
}

string		Tokenizer::seq(const char* seq)
{
	string			token;
	
	size_t		idx = str.find(seq, it - str.begin().base());
	if (idx != -1)
		token.assign(it, it + idx);
	it += idx + ft::strlen(seq);
	return token;
}

template <typename T>
void		print(T con)
{
	for (typename T::iterator it = con.begin(); it != con.end() ; ++it)
		cout << *it << endl;
}

// int		main()
// {
// 	string		a[5];
// 	a[0] = "/";
// 	a[1] = "/aa";
// 	a[2] = "/aa/bb";
// 	a[3] = "/aa/bb/cc";
// 	a[4] = "/aa/bb/cc/";


// 	for (int i = 0; i < 5 ; ++i)
// 	{
// 		Tokenizer		tok(a[i]);
// 		list<string>	ls = tok.path_segment();
// 		print(ls);
// 		cout << endl;
// 		cout << endl;
// 	}
// }
