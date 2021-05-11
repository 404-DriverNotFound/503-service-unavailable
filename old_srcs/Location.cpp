#include "../includes/Location.hpp"
#include "../includes/Utils.hpp"

// #define DBG

//------------------------------------------------------------------------------

Location::Location()
: ConfigSet()
{
}
//------------------------------------------------------------------------------

Location::Location(const Location& ref)
: ConfigSet(ref), 
cgi_extension(ref.cgi_extension),
auth_type(ref.auth_type),
auth(ref.auth)
{
}

Location::Location(std::deque<std::string>& lines)
: ConfigSet()
{
	autoindex = false;
	while (!lines.empty())
	{
		#ifdef DBG
		cout << lines.front() << endl;
		#endif

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
				map<string, uint32_t>::iterator	it_method = Method::method_flags.find(tokens.front());
				if (it_method == Method::method_flags.end())
				{
					#ifdef DBG
					cout << tokens.front() << endl;
					#endif
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
