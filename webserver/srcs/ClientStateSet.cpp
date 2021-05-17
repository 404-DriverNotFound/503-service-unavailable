#include "../includes/ClientStateSet.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/Utils.hpp"

//------------------------------------------------

ClientStateSet::ClientStateSet()
{
}

//------------------------------------------------

ClientStateSet::~ClientStateSet()
{
}

//------------------------------------------------

ClientState*	ClientStateSet::action(Client& ref)
{
	set_server(ref);
	map<string, string>::iterator	it_header = ref.get_httpreq().get_headers().find("TRANSFER_ENCODING");
	if (it_header != ref.get_httpreq().get_headers().end() && it_header->second.find("chunked"))
	{
		return	chunkedbody;
	}
	else
	{
		return	body;
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
	map<string, ConfigServer>::iterator	it_server = ref.get_servers().find(host);
	if (it_server == ref.get_servers().end())
	{
		cout << "Server Not Found" << endl;
		ref.get_httpres().set_status_code(404);
		return ;
	}
	ref.set_server(&it_server->second);
}

//------------------------------------------------

// void	ClientStateSet::set_location(Client& ref)
// {
// 	map<string, ConfigLocation>::iterator	it_location = ref.get_server().get_locations().find("/");
// }

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
	if (ref.get_location().get_auth_type != client_auth_type)
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

// void	ClientStateSet::set_path(Client& ref)
// {
	
// }

//------------------------------------------------

// void	ClientStateSet::set_method(Client& ref)
// {
// 	if (!(ref.get_location().get_method() & (1 << ref.get_
// }
