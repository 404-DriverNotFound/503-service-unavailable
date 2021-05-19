#include "../includes/ClientStateMethod.hpp"
#include "../includes/ClientStateLoadBody.hpp"

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
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	if (ref.get_httpreq().get_path().get_flag() == Path::flag_cgi)
	{

	}


	if (method == "GET")
	{
		res.set_status_code(200);
		res.get_stream() << res.get_start_line();
		res.get_stream() << res.get_content_length();
		res.get_stream() << "\r\n";
		res.set_len_msg(res_stream.size() + res.get_file_size());
		cout << "Method : " <<  "GET" << endl;
	}
	else if (method == "HEAD")
	{
		res.get_stream() << res.get_start_line();
		res.get_stream() << res.get_content_length();
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
	return	loadbody;
}