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

string		Tokenizer::seq(const char* seq)
{
	string			token;
	
	size_t		idx = str.find(seq, it - str.begin().base());
	if (idx != -1)
		token.assign(it, it + idx);
	it += idx + ft::strlen(seq);
	return token;
}

/*
int		main()
{
	string		str = make_str(n, k);
	Tokenizer	tokenizer(str);

	string token0 = tokenizer.chr('s');
	string token1 = tokenizer.chr('s');
	string token2 = tokenizer.chr('s');
}
*/