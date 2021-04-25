#pragma once
#include <map>
#include <string>
#include "Utils.hpp"

enum e_method
{
	CONNECT = 0,
	DELETE,
	GET,
	HEAD,
	OPTIONS,
	PATCH,
	POST,
	PUT,
	TRACE,
	NUM_METHOD
};

using std::cout;
using std::endl;
using std::string;
using std::map;

struct	Method
{
	static map<string, uint16_t>	method_num;
	static map<string, uint16_t>	method_flags;
	static string					method_strings[NUM_METHOD];

	static string&					get_method(uint16_t num);
	static uint16_t					get_method(const string& method_name);
	static string					get_allow(uint16_t flag);

	static void						init_method_num();
	static void						init_method_flags();
	static void						init_method_strings();
};