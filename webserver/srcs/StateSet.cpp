#include "../includes/StateSet.hpp"
#include "../includes/StateBody.hpp"
#include "../includes/StateChunkedBodyLen.hpp"
#include "../includes/StateMethod.hpp"
#include "../includes/Path.hpp"
#include "../includes/Utils.hpp"

//------------------------------------------------------------------------------

StateSet::StateSet()
{
	len = 0xffff;
}

//------------------------------------------------------------------------------

StateSet::~StateSet()
{
}

//------------------------------------------------------------------------------

State*	StateSet::action(Client& ref)
{
	// cout << "Set : " << __func__ << endl;
	set_server(ref);
	set_location(ref);
	check_auth(ref);
	check_method(ref);
	set_file(ref);
	
	return check_transfer_type(ref);
}

//------------------------------------------------------------------------------

State*	StateSet::check_transfer_type(Client& ref)
{
	const string&	trf = ref.get_httpreq().get_header("TRANSFER_ENCODING");
	if (trf == "chunked")
	{
		return	chunkedbody_len;
	}
	else
	{
		const string&	tmp = ref.get_httpreq().get_header("CONTENT_LENGTH");
		if (tmp.empty())
		{
			return method;
		}
		else
		{
			ref.get_httpreq().get_stream().set_pass_remain(ft::atoi(tmp));
			return	body;
		}
	}
}

//------------------------------------------------------------------------------

void	StateSet::set_server(Client& ref)
{
	map<string, string>::iterator	it_header = ref.get_httpreq().get_headers().find("HOST");
	if (it_header == ref.get_httpreq().get_headers().end())
	{
		// cout << "Host Empty" << endl;
		throw 400;
	}

	string						host;
	string::const_iterator		it_string = it_header->second.begin();
	ft::get_chr_token(it_header->second, it_string, host, ':');
	map<string, ConfigServer>::const_iterator	it_server = ref.get_servers().find(host);
	if (it_server == ref.get_servers().end())
	{
		// cout << "Server Not Found" << endl;
		throw 404;
	}
	ref.set_server(it_server->second);
}

//------------------------------------------------------------------------------

void	StateSet::set_location(Client& ref)
{
	// cout << __func__ << endl;
	typedef const map<string, ConfigLocation>&			location_container;
	typedef map<string, ConfigLocation>::const_iterator	location_iterator;

	string				location_name = ref.get_httpreq().get_path().get_location_name();
	location_container	locations = ref.get_server().get_locations();
	location_iterator	it_location = locations.find(location_name);

	if (it_location == locations.end())	// if not found
	{
		// find: "/"
		it_location = locations.find("/");
		if (it_location == locations.end())
		{
			throw 404;
		}
		location_name = "/";
	}
	const ConfigLocation&	location = it_location->second;
	ref.set_location(location);
	if (location_name == "/")
	{
		ref.get_httpreq().set_root_front(location.get_root(), location.get_cgi_extensions());
	}
	else
	{
		ref.get_httpreq().set_root_replace(location.get_root(), location.get_cgi_extensions());
	}
}

//------------------------------------------------------------------------------

void	StateSet::check_auth(Client& ref)
{
	if (ref.get_location().get_auth().empty())
		return;
	
	string&					line_auth = ref.get_httpreq().get_headers()["AUTHORIZATION"];
	string::const_iterator	it = line_auth.begin();
	string					client_auth_type;
	string					client_auth;

	ft::get_chr_token(line_auth, it, client_auth_type, ' ');
	if (ref.get_location().get_auth_type() != client_auth_type)
	{
		throw 401;
	}
	ft::get_chr_token(line_auth, it, client_auth, ' ');
	if (ref.get_location().get_auth() != client_auth)
	{
		throw 401;
	}
}

//------------------------------------------------------------------------------

void	StateSet::check_method(Client& ref)
{
	const set<string>&	method = ref.get_location().get_method();
	set<string>::const_iterator	it = method.find(ref.get_httpreq().get_method());
	if (it == method.end())
		throw 405;
}

//------------------------------------------------------------------------------

void	StateSet::set_file(Client& ref)
{
	// cout << __func__ << ": flag: " << ref.get_httpreq().get_path().get_flag() <<  endl;

	switch(ref.get_httpreq().get_path().get_flag())
	{
		case Path::flag_cgi:
			case_cgi(ref);
			break;

		case Path::flag_dir:
			case_dir(ref);
			break;

		case Path::flag_file:
			case_file(ref);
			break;

		case Path::flag_not_exist:
			case_not_exist(ref);
			break;
	}
}

//------------------------------------------------------------------------------

void	StateSet::case_cgi(Client& ref)
{
	HttpReq&				req = ref.get_httpreq();
	HttpRes&				res = ref.get_httpres();
	
	req.set_file(File::o_create);	// req: temp file
	res.set_file(File::o_create);	// res: temp file
}

void	StateSet::case_dir(Client& ref)
{
	const ConfigLocation&	location = ref.get_location();
	HttpReq&				req = ref.get_httpreq();
	HttpRes&				res = ref.get_httpres();
	const string&			path = req.get_path().get_path_translated();

	if (ref.get_method() != "GET" && ref.get_method() != "HEAD")
		req.set_file(File::o_create);	// req: temp file
	if (req.set_index_page(location.get_index_page()) == false) // index page not found
	{
		if (location.get_autoindex() == true)	// autoindex on?
		{
			res.set_file(File::o_create);	// temp file && autoindex
			res.set_autoindex_page(location, path);
		}
		else // autoindex off
		{
			throw 404;
		}
	}
	else
	{
		res.set_file(path);
	}
}

void	StateSet::case_file(Client& ref)
{
	HttpReq&				req = ref.get_httpreq();
	HttpRes&				res = ref.get_httpres();
	const string&			path = req.get_path().get_path_translated();
	
	if (req.get_method() == "GET")
	{
		res.set_file(path, File::o_read);
	}
	else if (req.get_method() == "PUT")
	{
		req.set_file(path, File::o_create);
		res.set_file(File::o_create);
	}
	else if (req.get_method() == "POST")
	{
		req.set_file(path, File::o_append);
		res.set_file(File::o_create);
	}
	else if (req.get_method() == "DELETE")
	{
		req.set_file(File::o_create);
		res.set_file(File::o_create);
		// ft::rm_df(path.c_str());
	}
	else if (req.get_method() == "HEAD")
	{
		res.set_file(path, File::o_read);
	}

}

void	StateSet::case_not_exist(Client& ref)
{
	HttpReq&				req = ref.get_httpreq();
	HttpRes&				res = ref.get_httpres();
	const string&			path = req.get_path().get_path_translated();

	// cout << "Path: " << path << endl;
	if (req.get_method() == "PUT")
	{
		req.set_file(path, File::o_create);
		res.set_file(File::o_create);
	}
	else if (req.get_method() == "POST")
	{
		req.set_file(path, File::o_append);
	}
	else
	{
		throw 404;
	}
}
