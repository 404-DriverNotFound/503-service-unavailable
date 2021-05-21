#pragma once
#include "Utils.hpp"
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
		list<string>	chr_list(char sep);
		list<string>	path_segment();
		string			rest();
		void			iter();
};