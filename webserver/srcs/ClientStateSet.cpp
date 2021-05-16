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
}

//------------------------------------------------

void	ClientStateSet::set_location(Client& ref)
{
	
}

//------------------------------------------------

void	ClientStateSet::check_auth(Client& ref)
{
	
}

//------------------------------------------------

void	ClientStateSet::set_path(Client& ref)
{
	
}

//------------------------------------------------

void	ClientStateSet::set_method(Client& ref)
{
	
}
