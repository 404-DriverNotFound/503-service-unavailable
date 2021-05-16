#include <map>
#include <set>
#include <cstring>
#include <vector>
#include <iostream>
#include "Time.hpp"

using std::string;
using std::vector;
using std::map;
using std::set;
using std::cout;
using std::endl;

int		main()
{
	set<string>		s;
	vector<string>			v;


	// m["get"] = "string_get";
	// m["put"] = "string_put";
	// m["post"] = "string_post";
	// m["head"] = "string_head";
	// m["delete"] = "string_delete";
	// m["option"] = "string_option";
	// m["trace"] = "string_trace";
	
	s.insert("get");
	s.insert("put");
	s.insert("post");
	s.insert("head");
	s.insert("delete");
	s.insert("option");
	s.insert("trace");
	
	v.push_back("get");
	v.push_back("put");
	v.push_back("post");
	v.push_back("head");
	v.push_back("delete");
	v.push_back("option");
	v.push_back("trace");

	size_t	memsize = 10000000;
	char*	tmp0 = new char[memsize];
	char*	tmp1 = new char[memsize];
	{
		Time	t0;
		for (int i = 0 ; i < 1000 ; i++)
		{
			s.find("head");
			memset(tmp0, 0, memsize);
		}
		Time	t1;
		cout << (t1 -t0) << endl;
	}
	{
		Time	t0;
		for (int i = 0 ; i < 1000 ; i++)
		{
			for (vector<string>::iterator it = v.begin() ; it != v.end() ; ++it)
			{
				if (*it == "head")
					break;
			}
			memset(tmp1, 0, memsize);
		}
		Time	t1;
		cout << (t1 -t0) << endl;
	}

}