#include "../includes/Location.hpp"
#include "../includes/Utils.hpp"
#define DBG

//------------------------------------------------------------------------------

Location::Location()
: ConfigSet()
{
	//TODO
}
//------------------------------------------------------------------------------

Location::Location(const Location& ref)
: ConfigSet(ref), 
cgi_extension(ref.cgi_extension),
auth_type(ref.auth_type),
auth(ref.auth)
{
	//TODO
}

Location::Location(std::deque<std::string>& lines)
: ConfigSet()
{
	autoindex = false;
	while (!lines.empty())
	{
		// cout << lines.front() << endl;

		int		indent = ft::count_chr(lines.front(), '\t');
		lines.front().erase(lines.front().begin(), lines.front().begin() + indent);
		list<string>	tokens = ft::get_set_token(lines.front(), " ");
		string&			key = tokens.front();
		if (key == "location" || key == "server")
		{
			break;
		}
		if (tokens.size() < 2)
		{
			throw ConfigSet::InvalidConfig();
		}
		string&			val = *++tokens.begin();

		if (key == "root")
		{
			root = val;
		}
		else if (key == "server_name")
		{
			name = val;
		}
		else if (key == "error_page")
		{
			for (list<string>::iterator it = ++tokens.begin() ; it != tokens.end() ; ++it)
			{
				error_page.insert(*it);
			}
		}
		else if (key == "index")
		{
			for (list<string>::iterator it = ++tokens.begin() ; it != tokens.end() ; ++it)
			{
				index.insert(*it);
			}
		}
		else if (key == "method")
		{

			tokens.pop_front();
			while (!tokens.empty())
			{
				map<string, uint16_t>::iterator	it_method = Method::method_flags.find(tokens.front());
				if (it_method == Method::method_flags.end())
				{
					cout << tokens.front() << endl;
					throw InvalidMethod();
				}
				method |= it_method->second;
				tokens.pop_front();
			}
		}
		else if (key == "head_length")
		{
			head_length = ft::atoi(val);
		}
		else if (key == "body_length")
		{
			body_length = ft::atoi(val);
		}
		else if (key == "autoindex")
		{
			if (val == "on")
				autoindex = true;
			else if (val == "off")
				autoindex = false;
			else
				throw ConfigSet::InvalidConfig();
		}
		else if (key == "timeout")
		{
			timeout = ft::atoi(val);
		}
		else if (key == "cgi_extension")
		{
			tokens.pop_front();
			while (!tokens.empty())
			{
				cgi_extension.insert(tokens.front());
				tokens.pop_front();
			}
		}
		else if (key == "auth")
		{
			char	buffer[1001];
			int		len;
			int		fd_auth;
			
			fd_auth = open(val.c_str(), O_RDONLY);
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
		else
		{
			throw ConfigSet::InvalidConfig();
		}
		lines.pop_front();
	}


	// cout << token.front() << endl;
	// std::string	str = " \t";
	// char	*base, *seq = str.begin().base();

	// method = 0;
	// base = token[0].begin().base();
	// location += base + 10;
	// token.pop_front();
	// base = token[0].begin().base();
	// while (!token.empty() && !ft::strncmp(base, "\t\t", 2))
	// {
	// 	base += 2;
	// 	if (!ft::strncmp(base, "root", 4))
	// 	{
	// 		root += (base + 5);
	// 	}
	// 	else if (!ft::strncmp(base, "cgi", 3))
	// 	{
	// 		cgi += (base + 4);
	// 	}
	// 	else if (!ft::strncmp(base, "auth", 4))
	// 	{
	// 		char	buffer[1001];
	// 		int		len;
	// 		int		fd_auth;
			
	// 		fd_auth = open(base + 5, O_RDONLY);
	// 		if (fd_auth < 0)
	// 			throw AuthFailed();
	// 		while ((len = read(fd_auth, buffer, 1000)) > 0)
	// 		{
	// 			buffer[len] = 0;
	// 			auth.insert(auth.end(), buffer, buffer + len);
	// 		}
	// 		if (len < 0)
	// 			throw AuthFailed();
	// 		auth_type = "Basic";
	// 		close(fd_auth);
	// 	}
	// 	else if (!ft::strncmp(base, "error_page", 10))
	// 	{
	// 		std::string::const_iterator	it;
	// 		std::string					temp;

	// 		it = token[0].begin() + 13;
	// 		while (ft::get_set_token(token[0], it, temp, seq))
	// 		{
	// 			error_page.insert(temp);
	// 		}
	// 		error_page.insert(temp);
	// 	}
	// 	else if (!ft::strncmp(base, "index", 5))
	// 	{
	// 		std::string::const_iterator	it;
	// 		std::string					temp;

	// 		it = token[0].begin() + 8;
	// 		while (ft::get_set_token(token[0], it, temp, seq))
	// 		{
	// 			index.insert(temp);
	// 		}
	// 		index.insert(temp);
	// 	}
	// 	else if (!ft::strncmp(base, "method", 6))
	// 	{
	// 		std::string::const_iterator	it;
	// 		std::string					temp;
	// 		bool						is_remain;

	// 		it = token[0].begin() + 9;
	// 		while (42)
	// 		{
	// 			is_remain = ft::get_set_token(token[0], it, temp, seq);
	// 			map<string, uint16_t>::iterator	it_method = Method::method_flags.find(temp);
	// 			if (it_method == Method::method_flags.end())
	// 				throw InvalidMethod();
	// 			method |= it_method->second;
	// 			if (!is_remain)
	// 				break;
	// 		}
	// 	}
	// 	else if (!ft::strncmp(base, "body_length", 11))
	// 	{
	// 		body_length = static_cast<u_int64_t>(atoi(base + 12));
	// 	}
	// 	else if (!ft::strncmp(base, "autoindex", 8))
	// 	{
	// 		if (!ft::strncmp(base + 9, " on", 0))
	// 			autoindex = true;
	// 		else
	// 			autoindex = false;
	// 	}
	// 	token.pop_front();
	// 	base = token[0].begin().base();
	// }
}

//------------------------------------------------------------------------------

Location::~Location()
{
}

std::ostream&	operator<<(std::ostream& os, Location& ref) {
	os 	<< "		root: " << ref.root << std::endl
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
