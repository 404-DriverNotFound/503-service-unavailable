#include "../includes/Location.hpp"
#include "../includes/Utils.hpp"
#define DBG

//------------------------------------------------------------------------------

Location::Location(const ConfigSet& ref)
{
	//TODO
}

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
		else if (!ft::strncmp(base, "cgi", 3))
		{
			cgi += (base + 4);
		}
		else if (!ft::strncmp(base, "auth", 4))
		{
			char	buffer[1001];
			int		len;
			int		fd_auth;
			
			fd_auth = open(base + 5, O_RDONLY);
			if (fd_auth < 0)
				throw AuthFailed();
			while ((len = read(fd_auth, buffer, 1000)) > 0)
			{
				buffer[len] = 0;
				auth.insert(auth.end(), buffer, buffer + len);
			}
			if (len < 0)
				throw AuthFailed();
			auth_type = "Basic";
			close(fd_auth);
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
				map<string, uint16_t>::iterator	it_method = Method::method_flags.find(temp);
				if (it_method == Method::method_flags.end())
					throw InvalidMethod();
				method |= (1 << it_method->second);
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
	os <<"		location " << ref.location << std::endl
		<< "		root: " << ref.root << std::endl
		<<"		error_page: ";
	std::set<std::string>::iterator	it;

	it = ref.error_page.begin();
	for (;it != ref.error_page.end();++it)
		os << *it << " ";
	os << std::endl
		<< "		index: ";
	it = ref.index.begin();
	for (; it != ref.index.end();++it)
		os << *it << " ";
	os << std::endl
		<< "		body_length: " << ref.body_length << std::endl;
	os << "		method: " << ref.method;
	return os;
}

const char*		Location::AuthFailed::what() const throw()
{
	return "Auth Failed";
}

const char*		Location::InvalidMethod::what() const throw()
{
	return "Invalid Method";
}
