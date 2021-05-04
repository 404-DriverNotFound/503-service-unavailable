#include "../includes/Method.hpp"

map<string, uint16_t>	Method::method_num;
map<string, uint16_t>	Method::method_flags;
string					Method::method_strings[NUM_METHOD];

		Method::Method()
{}

		Method::~Method()
{}


string&		Method::get_method(uint16_t num)
{
	return method_strings[num];
}

uint16_t	Method::get_method(const string& method_name)
{
	return method_num[method_name];
}

string		Method::get_allow(uint16_t flag)
{
	string	line("Allow: ");
	int		k = 0;
	for (int i = 0 ; flag ; ++i)
	{
		if (flag & 1)
		{
			if (k)
				line += ", ";
			line += get_method(i);
			k++;
		}
	}
	line += "\r\n";
	return line;
}

void		Method::init_method_num()
{
	method_num["CONNECT"] = CONNECT;
	method_num["DELETE"] = DELETE;
	method_num["GET"] = GET;
	method_num["HEAD"] = HEAD;
	method_num["OPTIONS"] = OPTIONS;
	method_num["PATCH"] = PATCH;
	method_num["POST"] = POST;
	method_num["PUT"] = PUT;
	method_num["TRACE"] = TRACE;
}

void		Method::init_method_flags()
{
	method_flags["CONNECT"] = (1 << CONNECT);
	method_flags["DELETE"] = (1 << DELETE);
	method_flags["GET"] = (1 << GET);
	method_flags["HEAD"] = (1 << HEAD);
	method_flags["OPTIONS"] = (1 << OPTIONS);
	method_flags["PATCH"] = (1 << PATCH);
	method_flags["POST"] = (1 << POST);
	method_flags["PUT"] = (1 << PUT);
	method_flags["TRACE"] = (1 << TRACE);
}

void		Method::init_method_strings()
{
	method_strings[CONNECT] = string("CONNECT");
	method_strings[DELETE] = string("DELETE");
	method_strings[GET] = string("GET");
	method_strings[HEAD] = string("HEAD");
	method_strings[OPTIONS] = string("OPTIONS");
	method_strings[PATCH] = string("PATCH");
	method_strings[POST] = string("POST");
	method_strings[PUT] = string("PUT");
	method_strings[TRACE] = string("TRACE");
}
