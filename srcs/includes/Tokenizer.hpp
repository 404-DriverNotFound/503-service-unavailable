#pragma once
#include "ClassModule.hpp"
#include <string>
#include <list>

using std::string;
using std::list;

class Tokenizer
{
	public:
		string					str;
		const char*				it;
		const char*				end;

	public:
		Tokenizer();
		Tokenizer(string& str);
		Tokenizer(const string& str);
		Tokenizer(const Tokenizer& ref);
		~Tokenizer();
		Tokenizer&	operator=(const Tokenizer& ref);

		
		string			seq(const char* sep);
		string			set(const char* sep);
		string			chr(char sep);
		string			chr2(char sep);
		list<string>	chr_list(char sep);
		void			iter();
};