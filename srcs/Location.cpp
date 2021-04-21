#include "Location.hpp"
#include "Utils.hpp"
//------------------------------------------------------------------------------
Method Location::methodSet[9] = {
	{"CONNECT", 1 << CONNECT},
	{"DELETE", 1 << DELETE},
	{"GET", 1 << GET},
	{"HEAD", 1 << HEAD},
	{"OPTIONS", 1 << OPTIONS},
	{"PATCH", 1 << PATCH},
	{"POST", 1 << POST},
	{"PUT", 1 << PUT},
	{"TRACE", 1 << TRACE}
};

Location::Location(std::deque<std::string>& token)
{
	std::string	str = " \t";
	char	*base, *seq = str.begin().base();

	base = token[0].begin().base();
	location += base + 10;
	token.pop_front();
	base = token[0].begin().base();
	while (!token.empty() && !ft::strncmp(base, "\t\t", 2))
	{
		base += 2;
		if (!ft::strncmp(base, "root", 4))
		{
			root += (base + 5);
		}
		if (!ft::strncmp(base, "cgi", 3))
		{
			root += (base + 3);
		}
		else if (!ft::strncmp(base, "error_page", 10))
		{
			std::string::const_iterator	it;
			std::string					temp;

			it = token[0].begin() + 13;
			while (ft::get_set_token(token[0], it, temp, seq))
			{
				error_page.insert(temp);
			}
			error_page.insert(temp);
		}
		else if (!ft::strncmp(base, "index", 5))
		{
			std::string::const_iterator	it;
			std::string					temp;

			it = token[0].begin() + 8;
			while (ft::get_set_token(token[0], it, temp, seq))
			{
				index.insert(temp);
			}
			index.insert(temp);
		}
		else if (!ft::strncmp(base, "method", 6))
		{
			std::string::const_iterator	it;
			std::string					temp;

			it = token[0].begin() + 9;
			while (ft::get_set_token(token[0], it, temp, seq))
			{
				for (int idx=0;idx<9;idx++)
					if (temp == methodSet[idx].str) {
						method |= methodSet[idx].flag;
						break;
					}
			}
		}
		else if (!ft::strncmp(base, "body_length", 11))
		{
			body_length = static_cast<u_int64_t>(atoi(base + 12));
		}
		else if (!ft::strncmp(base, "autoindex", 8))
		{
			if (!ft::strncmp(base + 9, " on", 0))
				autoindex = true;
			else
				autoindex = false;
		}
		token.pop_front();
		base = token[0].begin().base();
	}
}

//------------------------------------------------------------------------------

Location::~Location()
{
}

std::ostream&	operator<<(std::ostream& os, Location& ref) {
	os <<"location " << ref.location << std::endl
		<< "root: " << ref.root << std::endl
		<<"error_page: ";
	std::set<std::string>::iterator	it;

	it = ref.error_page.begin();
	for (;it != ref.error_page.end();++it)
		os << *it << " ";
	os << std::endl
		<< "index: ";
	it = ref.index.begin();
	for (; it != ref.index.end();++it)
		os << *it << " ";
	os << std::endl
		<< "body_length: " << ref.body_length << std::endl;
	os << "method: " << ref.method;
	return os;
}
