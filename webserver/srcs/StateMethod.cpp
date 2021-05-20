#include "../includes/StateMethod.hpp"
#include "../includes/StateLoadBody.hpp"

StateMethod::StateMethod()
{
	len = 0xffff;
}

StateMethod::~StateMethod()
{
}

State* StateMethod::action(Client& ref)
{
	cout << "---------------------------------------\n";
	cout << "Method : " <<  __func__ << endl;
	const string&	method = ref.get_httpreq().get_method();
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	switch (ref.get_httpreq().get_path().get_flag())
	{
	case Path::flag_cgi:
		break;

	case Path::flag_dir:
		res.set_status_code(200);
		res.get_stream() << res.get_start_line();
		res.get_stream() << res.get_content_length();
		res.get_stream() << "\r\n";
		if (method != "HEAD")
		{
			res.set_len_msg(res_stream.size() + res.get_file_size());
		}
		else
		{
			res.set_len_msg(res_stream.size());
		}
		cout << "Method : " <<  "GET" << endl;
		break;

	case Path::flag_file:
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
		break;

	case Path::flag_not_exist:
		/* code */
		break;
	
	default:
		break;
	}

	cout << "---------------------------------------\n";
	return	loadbody;
}