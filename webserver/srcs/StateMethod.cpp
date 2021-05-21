#include "../includes/StateMethod.hpp"
#include "../includes/StateLoadBody.hpp"
#include "../includes/StateWait.hpp"
#include "../includes/StateCgi.hpp"

StateMethod::StateMethod()
{
	len = 0xffff;
}

StateMethod::~StateMethod()
{
}

State* StateMethod::action(Client& ref)
{
	// cout << "---------------------------------------\n";
	// cout << "Method : " <<  __func__ << endl;
	const string&	method = ref.get_httpreq().get_method();
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	switch (ref.get_httpreq().get_path().get_flag())
	{
	case Path::flag_cgi:
		return case_cgi(ref);
		break;

	case Path::flag_dir:
		return case_dir(ref);
		break;

	case Path::flag_file:
		return case_file(ref);
		break;

	case Path::flag_not_exist:
		return case_not_exist(ref);
		break;
	
	default:
		break;
	}

	// cout << "---------------------------------------\n";
	return	loadbody;
}

State*		StateMethod::case_cgi(Client& ref)
{
	const string&	method = ref.get_httpreq().get_method();
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	ref.set_cgi();
	ref.make_meta_variable();
	ref.print_meta_variable();
	ref.run_cgi();
	return cgi;
}

State*		StateMethod::case_dir(Client& ref)
{
	if (ref.get_method() == "HEAD")
	{
		return method_head(ref);
	}
	else
	{
		return method_get(ref);
	}
}

State*		StateMethod::case_file(Client& ref)
{
	const string&	method = ref.get_method();

	if (method == "GET")
	{
		return method_get(ref);
	}
	else if (method == "HEAD")
	{
		return method_head(ref);
	}
	else if (method == "PUT")
	{
		return method_put(ref);
	}
	else if (method == "POST")
	{
		return method_put(ref);
	}
	else if (method == "DELETE")
	{

	}
	else
	{

	}
	return loadbody;
}

State*		StateMethod::case_not_exist(Client& ref)
{

	const string&	method = ref.get_method();

	if (method == "HEAD")
	{
		return method_head(ref);
	}
	else
	{
		return method_get(ref);
	}
}

State*		StateMethod::method_head(Client& ref)
{
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	res.set_status_code(200);
	res.get_stream() << res.get_start_line();
	res.get_stream() << res.get_content_length();
	res.get_stream() << "\r\n";
	// STOP
	// if (res.get_status_code() >= 400)
	// {
	// 	res.set_len_msg(res_stream.size() + res.get_file_size());
	// 	return loadbody;
	// }
	// else
	{
		res.set_len_msg(res_stream.size());
		return waiting;
	}
}

State*		StateMethod::method_get(Client& ref)
{
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	res.set_status_code(200);
	res.get_stream() << res.get_start_line();
	res.get_stream() << res.get_content_length();
	res.get_stream() << "\r\n";
	res.set_len_msg(res_stream.size() + res.get_file_size());

	return loadbody;
}

State*		StateMethod::method_put(Client& ref)
{
	HttpReq&		req = ref.get_httpreq();
	HttpRes&		res = ref.get_httpres();
	Stream&			res_stream = res.get_stream();

	res.set_status_code(201);
	res.get_stream() << res.get_start_line();
	res.get_stream() << res.get_content_length();
	res.get_stream() << "\r\n";
	res.set_len_msg(res_stream.size() + res.get_file_size());

	return loadbody;
}
