#include "../includes/ClientStateSet.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBody.hpp"
#include "../includes/Path.hpp"
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
	ref.set_server(it_server->second);
}

//------------------------------------------------

void	ClientStateSet::set_location(Client& ref)
{
	typedef map<string, ConfigLocation>::const_iterator	location_iterator;

	const string&		location_name = ref.get_httpreq().get_path().get_segments().front();
	location_iterator	it_location = ref.get_server().get_locations().find(location_name);

	if (it_location == ref.get_server().get_locations().end())
	{
		if ((it_location = ref.get_server().get_locations().find("/")) == ref.get_server().get_locations().end())
			ref.get_httpres().set_status_code(404);
			return ;
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
	// Path&					path = req.get_path();


	switch(ref.get_httpreq().get_path().get_flag())
	{
		case Path::flag::flag_cgi:
			req.set_file(File::flag::o_create);
			res.set_file(File::flag::o_create);
			break;
		case Path::flag::flag_dir:
			// ref.get_httpreq().set_file(File::flag::o_create);
			
			if (req.get_path().set_index(ref.get_location().get_index_page())) // 인덱스 찾기 실패
			{
				if (location.get_autoindex() == true)
				{
					/* 오토인덱스 페이지 생성 */
				}
				else
				{
					/* 에러페이지 */
				}

			}

			ref.get_httpres().set_file(ref.get_httpreq().get_path().get_path_translated());

			break;
		case Path::flag::flag_file:
			
			ref.get_httpreq().set_file();
			ref.get_httpres().set_file();
			break;
		case Path::flag::flag_not_exist:
			ref.get_httpreq().set_file();
			ref.get_httpres().set_file();
			break;
		default:
			break;
	}
	if (ref.get_httpreq().get_path().is_cgi() == true)
	{
		// 임시파일
	}
	else if (ref.get_httpreq().get_path().is_dir() == true)
	{
		// get 요청 처럼 처리
		// 오토인덱스 : on 일때 index.html이 없으면 디렉토리 리스트
		
	}
	else if (ref.get_httpreq().get_path().is_file() == true)
	{
		// 메서드별로 처리
	}
	else
	{
		// get ? (오토인덱스 on ? 오토인덱스페이지 : 404) : 404
		// put ? (덮어쓰기로 생성)
		// post ? (이어쓰기로 생성)
		// delete 404
	}
}

//------------------------------------------------

// void	ClientStateSet::set_method(Client& ref)
// {
// 	if (!(ref.get_location().get_method() & (1 << ref.get_
// }
