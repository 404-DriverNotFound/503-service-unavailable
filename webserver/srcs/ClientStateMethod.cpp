#include "../includes/ClientStateMethod.hpp"
#include "../includes/ClientStateWait.hpp"

ClientStateMethod::ClientStateMethod()
{
	len = 0xffff;
}

ClientStateMethod::~ClientStateMethod()
{
}

ClientState* ClientStateMethod::action(Client& ref)
{
	cout << "Method : " <<  __func__ << endl;
	const string&	method = ref.get_httpreq().get_method();

	if (ref.get_httpreq().get_path().get_flag() == Path::flag_cgi)
	{

	}


	if (method == "GET")
	{
		ref.get_httpres().get_stream() << ref.get_httpres().get_start_line();
		ref.get_httpres().get_stream() << ref.get_httpres().get_content_length();
	}
	else if (method == "HEAD")
	{
		ref.get_httpres().get_stream() << ref.get_httpres().get_start_line();
		ref.get_httpres().get_stream() << ref.get_httpres().get_content_length();
	}
	else if (method == "PUT")
	{

	}
	else if (method == "POST")
	{

	}
	else if (method == "DELETE")
	{
		
	}
	else
	{

	}
	return	waiting;
}