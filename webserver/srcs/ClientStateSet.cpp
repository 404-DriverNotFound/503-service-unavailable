#include "../includes/ClientStateSet.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/ClientStateMethod.hpp"
#include "../includes/Path.hpp"
#include "../includes/Utils.hpp"

//------------------------------------------------

ClientStateSet::ClientStateSet()
{
	len = 0xffff;
}

//------------------------------------------------

ClientStateSet::~ClientStateSet()
{
}

//------------------------------------------------

ClientState*	ClientStateSet::action(Client& ref)
{
	try
	{
		set_server(ref);
		set_location(ref);
		check_auth(ref);
		check_method(ref);
		set_file(ref);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	const string&	trf = ref.get_httpreq().get_header("TRANSFER_ENCODING");
	if (trf == "chunked")
	{
		return	chunkedbody;
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

//------------------------------------------------

void	ClientStateSet::set_server(Client& ref)
{
	map<string, string>::iterator	it_header = ref.get_httpreq().get_headers().find("HOST");
	if (it_header == ref.get_httpreq().get_headers().end())
	{
		cout << "Host Empty" << endl;
		ref.get_httpres().set_status_code(400);
		return ;
	}

	string						host;
	string::const_iterator		it_string = it_header->second.begin();
	ft::get_chr_token(it_header->second, it_string, host, ':');
	// Webserver::port_iterator = ref.get_servers().find(host);
	map<string, ConfigServer>::const_iterator	it_server = ref.get_servers().find(host);
	if (it_server == ref.get_servers().end())
	{
		cout << "Server Not Found" << endl;
		ref.get_httpres().set_status_code(404);
		return ;
	}
	ref.set_server(it_server->second);
}

//------------------------------------------------

void	ClientStateSet::set_location(Client& ref)
{
	typedef map<string, ConfigLocation>::const_iterator	location_iterator;

	string				location_name = "/";
	location_name += ref.get_httpreq().get_path().get_segments().front();
	location_iterator	it_location = ref.get_server().get_locations().find(location_name);

	if (it_location == ref.get_server().get_locations().end())
	{
		if ((it_location = ref.get_server().get_locations().find("/")) == ref.get_server().get_locations().end())
		{
			ref.get_httpres().set_status_code(404);
			return ;
		}
	}
	ref.set_location(it_location->second);
	ref.get_httpreq().get_path().set_root(it_location->second.get_root());
}

//------------------------------------------------

void	ClientStateSet::check_auth(Client& ref)
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
		ref.get_httpres().set_status_code(401);
		return ;
	}
	ft::get_chr_token(line_auth, it, client_auth, ' ');
	if (ref.get_location().get_auth() != client_auth)
	{
		ref.get_httpres().set_status_code(401);
		return ;
	}
}

//------------------------------------------------

void	ClientStateSet::check_method(Client& ref)
{
	const set<string>&	method = ref.get_location().get_method();
	set<string>::const_iterator	it = method.find(ref.get_httpreq().get_method());
	if (it == method.end())
		ref.get_httpres().set_status_code(405);	// method not allowed
}

//------------------------------------------------

void	ClientStateSet::set_file(Client& ref)
{
	const ConfigLocation&	location = ref.get_location();
	HttpReq&				req = ref.get_httpreq();
	HttpRes&				res = ref.get_httpres();
	const string&			path = req.get_path().get_path_translated();

	switch(ref.get_httpreq().get_path().get_flag())
	{
		case Path::flag_cgi:
			req.set_file(File::o_create);	// req: temp file
			res.set_file(File::o_create);	// res: temp file
			break;

		case Path::flag_dir:
			req.set_file(File::o_create);	// req: temp file
			if (req.set_index_page(location.get_index_page()) == false) // index page not found
			{
				if (location.get_autoindex() == true)	// autoindex on?
				{
					res.set_file(File::o_create);	// temp file && autoindex
					res.set_autoindex_page(path);
				}
				else // autoindex off
				{
					res.set_status_code(404);
					if (req.set_index_page(location.get_error_page()) == false)	// error page not found
					{
						res.set_file(File::o_create);
					}
					else
					{
						res.set_file(path);
					}
				}
			}
			else
			{
				res.set_file(path);
			}
			break;

		case Path::flag_file:
			if (req.get_method() == "GET")
			{
				res.set_file(path, File::o_read);
			}
			else if (req.get_method() == "PUT")
			{
				req.set_file(path, File::o_create);
			}
			else if (req.get_method() == "POST")
			{
				req.set_file(path, File::o_append);
			}
			else if (req.get_method() == "DELETE")
			{
				ft::rm_df(path.c_str());
			}
			else if (req.get_method() == "HEAD")
			{
			}
			break;

		case Path::flag_not_exist:
			if (req.get_method() == "PUT")
			{
				req.set_file(path, File::o_create);
			}
			else if (req.get_method() == "POST")
			{
				req.set_file(path, File::o_append);
			}
			else
			{
				res.set_status_code(404);
				if (req.set_index_page(location.get_error_page()) == false)	// error page not found
				{
					res.set_file(File::o_create);
				}
				else
				{
					res.set_file(path);
				}
			}
			break;
	}
}

//------------------------------------------------

// void	ClientStateSet::set_method(Client& ref)
// {
// 	if (!(ref.get_location().get_method() & (1 << ref.get_
// }
