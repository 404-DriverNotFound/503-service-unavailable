/*
복사생성자때문에, 소켓 초기화는 init함수로 외부에서 진행함
프로세스 함수 외부에서 셀렉트 결과에 따라 스트림in에 수신 데이터가 채워짐

================================================================================
프로세스:
스위치 (상태)
----------------------------------------------------------------------
시작줄:
	스트림으로부터 한 줄 얻기
	줄이 완벽하면 
		시작줄로 구문분석 한다
		상태를 헤더로 변경
	상태가 "시작줄" 상태 그대로이면 리턴
----------------------------------------------------------------------
헤더:
	스트림으로부터 한 줄 얻기
	if 내용이 없으면
		요청 바디의 길이가 0이면
			상태를 메시지 처리로 변경
		대기시간 체크, 초과했으면
			상태 = 메시지 처리
	else if 빈 줄이면
		상태를 메시지 처리로 변경
	else if 줄이 완벽하면
		헤더로 구문분석 한다 -> 일단은 헤더별로 값만 저장
		요청 바디의 길이는 별도로 저장한다.
	else ( == 줄이 불완전하면)
		리턴
	상태가 "헤더" 상태 그대로이면 리턴
----------------------------------------------------------------------
메시지 처리:
	서버 목록에서 서버 찾기(호스트 헤더)
	서버에서 로케이션 찾기(path_info의 첫 번째 경로조각이거나, 경로조각이 1개인 경우 "/")
		로케이션을 찾지 못하면 not_found
	권한 및 메서드 허용여부 확인
	확장자 확인. 확장자가 CGI가 필요한 확장자인 경우는 CGI 실행
		메타변수 생성
		CGI 초기화
		스트림in을 CGI로 연결
		CGI 시작
	상태를 메서드로 변경
	메서드 분기 함수 실행
----------------------------------------------------------------------
메서드:
	메서드 분기 함수 실행
	상태가 "메서드" 상태 그대로이면 리턴
----------------------------------------------------------------------
종료:



================================================================================
메서드 분기 함수:
스위치 (메서드)
----------------------------------------------------------------------
GET:
	
	if CGI (O):
		CGI실행
		빈 줄 나올 때까지 CGI.stream_out.getline()
			stream_out << 헤더를 파싱
	stream_out << 리소스
	파일 사이즈 확인 -> Content-Length 결정
	stream_out << 헤더 작성(시작줄, 헤더)
	상태 = 메시지 전달
----------------------------------------------------------------------
HEAD:
	
	if CGI (O):
		CGI실행
		빈 줄 나올 때까지 CGI.stream_out.getline()
	파일 사이즈 확인 -> Content-Length 결정
	stream_out << 헤더 작성(시작줄, 헤더)
	if CGI (O):
		stream_out << 헤더를 파싱
	상태 = 메시지 전달
----------------------------------------------------------------------
POST:
	if CGI (O)
		CGI실행
		chunked일 경우
			본문에서 길이 얻기
			길이가 완벽하면
				길이만큼 CGI에 본문 전달
			완벽하지 않으면
				남은 길이를 저장
		chunked 아닐 경우
			남은 콘텐츠 길이만큼 CGI에 본문 입력
		빈 줄 나올 때까지 CGI.stream_out.getline()
			stream_out << 헤더를 파싱	
	stream_out << 헤더 작성(시작줄, 헤더)

PUT:
	리소스 쓰기
DELETE:
	리소스 삭제
...




*/






#include "../includes/Client.hpp"
#define DBG




//------------------------------------------------------------------------------


			Client::Client(map<string, Server>& servers)
: servers(servers)
{
}

			Client::Client(const Client& x)
: servers(x.servers)
{
}

//------------------------------------------------------------------------------

			Client::~Client()
{
}

//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/socket.h>
void		Client::init(int fd)
{
	linger	a;
	a.l_onoff = 0;
	a.l_linger = 50000;
	alive.set_current();
	sock.accept(fd);
	setsockopt(sock.fd, SOL_SOCKET, SO_LINGER, &a, sizeof(linger));
	stream_in.init(10000, sock.fd, sock.fd);
	stream_out.init(10000, sock.fd, sock.fd);
	status = STATUS_START_LINE;
	status_proc = STATUS_INIT;
}

//------------------------------------------------------------------------------

void		Client::client_process(FdSet& r, FdSet& w)
{
	if ((Time() - alive).get_time_sec() > 3)
		status = STATUS_DONE;
	if (r.get(sock.fd))
	{
		switch (status)
		{
		case STATUS_START_LINE:
		{
			stream_in.fill(8000);	// TODO: header size
			r.del(sock.fd);
			break;
		}
		case STATUS_HEADER:
		{
			stream_in.fill(8000);	// TODO: header size
			r.del(sock.fd);
			break;
		}
		case STATUS_METHOD:
		{
			stream_in.fill(10000);
			r.del(sock.fd);
		}
		default:
			break;
		}
	}

	#ifdef DBG
	cout << "::Request::\n";
	#endif

	try
	{
		switch (status)
		{
			case STATUS_START_LINE:
				cout << "status: startline: " << line.size() << stream_in.size() << endl;
				if (stream_in.get_line(line))
				{
					cout << line << endl;
					req.set_start_line(line);
					status = STATUS_HEADER;
				}
				if (status == STATUS_START_LINE)
					return;
			case STATUS_HEADER:
				cout << "status: header: " << line.size() << stream_in.size() << endl;
				while (stream_in.get_line(line))
				{
					if (line.empty())
					{
						status = STATUS_CHECK_MSG;
						cout << "---===Header end===-----\n";
						break;
					}
					cout << line << endl;
					req.set_header(line);
				}
				if (status == STATUS_HEADER)
					return;
			case STATUS_CHECK_MSG:
				#ifdef DBG
				cout << "\n::Processing::\n" << line << endl << endl;
				#endif
				set_server();
				set_location();
				check_auth();
				check_method();
				translate_path();
				status = STATUS_METHOD;
			case STATUS_METHOD:
				cout << "status: method: " << line.size() << stream_in.size() << endl;
				process_method();
				if (status == STATUS_METHOD)
					return;
			case STATUS_SEND_MSG:
				#ifdef DBG
				cout << "\n::Sending msg::\n";
				#endif
				if (w.get(sock.fd))
				{
					::write(1, stream_out.buffers.front().start, 
						stream_out.buffers.front().end - stream_out.buffers.front().start);
					stream_out.pass();
					w.del(sock.fd);
				}
				if (stream_out.buffers.empty())
				{
					if (w.get(sock.fd))
					{
						::write(1, stream_out.buffers.front().start, 
							stream_out.buffers.front().end - stream_out.buffers.front().start);
						cgi.stream_out.pass();
						w.del(sock.fd);
					}
					if (cgi.stream_out.buffers.empty())
					{
						status = STATUS_DONE;
					}
				}
				if (status == STATUS_SEND_MSG)
					break;
			case STATUS_DONE:
				#ifdef DBG
				cout << "\n::Sending msg DONE::\n";
				#endif
				
				break;
		}
	}
	catch(int code)
	{

		string	error_page_name = ft::find(location->root, location->error_page);
		string	error_page;
		if (error_page_name.empty())
		{
			error_page_name = ft::find(server->root, server->error_page);
			error_page += server->root;
		}
		else
			error_page += location->root;
		error_page += "/";
		error_page += error_page_name;


		size_t	filesize = ft::file_size(error_page.c_str());
		int		fd_get = open(error_page.c_str(), O_RDONLY);

		res.status_code = code;
		stream_out.clear();
		stream_out << res.get_startline();
		stream_out << res.get_content_length(filesize);
		stream_out << "\r\n";
		
		stream_out.fd_in = fd_get;
		stream_out.fill(filesize);
		close(fd_get);

		status = STATUS_SEND_MSG;
		cout << "err code: " << code << endl;
		// usleep(1000000);
		// exit(0);
		client_process(r, w);
	}
	

}

//------------------------------------------------------------------------------

void		Client::set_server()
{
	cout << __func__ << endl;
	string				host;
	string::const_iterator	it = req.headers[HOST].begin();
	ft::get_chr_token(req.headers[HOST], it, host, ':');
	iterator_server	it_server = servers.find(host);
	if (it_server == servers.end())
	{
		cout << "server not found: " << host << endl;
		throw 404;
	}
	server = &it_server->second;
}

void		Client::set_location()
{
	iterator_location	it_location	= server->locations.find("/" + req.path.front());
	if (it_location == server->locations.end())
	{
		if ((it_location = server->locations.find("/")) == server->locations.end())
			throw 404;
	}
	else
	{
		req.path.pop_front();
	}
	location = &it_location->second;
}

void		Client::check_auth()
{
	cout << __func__ << endl;

	if (location->auth.empty())
		return ;

	string&					line_auth = req.headers[AUTHORIZATION];
	string::const_iterator	it = line_auth.begin();
	string					client_auth_type;
	string					client_auth;

	ft::get_chr_token(line_auth, it, client_auth_type, ' ');
	if (location->auth_type != client_auth_type)
		throw 401;	// TODO: bad request or unauthorized?
	ft::get_chr_token(line_auth, it, client_auth, ' ');
	if (location->auth != client_auth)
		throw 401;	// TODO: bad request or unauthorized?
}

void		Client::check_method()
{
	cout << __func__ << endl;
	if (!(location->method & (1 << req.method)))
	{
		cout << req.method << endl;
		cout << location->method << endl;
		throw 405; // Method not allowed
	}
}

void		Client::translate_path()
{
	cout << __func__ << endl;
	path_translated = location->root;
	list<string>::iterator 	it = req.path.begin();
	list<string>::iterator 	end = req.path.end();

	while (it != end)
	{
		path_translated.append("/");
		path_translated.append(*it);
		++it;
	}
}

//------------------------------------------------------------------------------

void		Client::process_method()
{
	// for (int i = 0; i < 17 ; i++)
	// {
	// 	cout << req.headers[i] << endl;
	// }

	switch (req.method)
	{
	case GET:
		process_get();
		break;
	case HEAD:
		process_head();
		break;
	case POST:
		process_post();
		break;
	case PUT:
		process_put();
		break;
	case DELETE:
		/* code */
		break;
	case OPTIONS:
		break;
	default:
		break;
	}
}
//------------------------------------------------------------------------------

size_t		Client::process_head_base()
{
	if (ft::is_dir(path_translated.c_str()))
	{
		string	file_name = ft::find(path_translated.c_str(), location->index);
		if (file_name.empty())
			throw 404;
		path_translated.append("/");
		path_translated.append(file_name);
	}

	#ifdef DBG
	cout << "- path: " << path_translated << endl;
	#endif

	size_t	filesize = ft::file_size(path_translated.c_str());
	if (filesize < 0)
		throw 404;

	res.status_code = 200;
	stream_out << res.get_startline();
	stream_out << res.get_content_length(filesize);
	stream_out << res.get_server();
	stream_out << "\r\n";
	return filesize;
}
//------------------------------------------------------------------------------

void		Client::process_get()
{
	#ifdef DBG
	cout << "- Method: GET\n";
	#endif

	size_t	filesize = process_head_base();

	int		fd_get = open(path_translated.c_str(), O_RDONLY);
	if (fd_get < 0)
		throw 404; // not found

	stream_out.fd_in = fd_get;
	stream_out.fill(filesize);
	close(fd_get);
	status = STATUS_SEND_MSG;
}
//------------------------------------------------------------------------------

void		Client::process_head()
{
	#ifdef DBG
	cout << "- Method: HEAD\n";
	cout << "- path: " << path_translated << endl;
	#endif

	process_head_base();
	
	status = STATUS_SEND_MSG;
}
//------------------------------------------------------------------------------

string		get_extention(const string& p)
{
	ssize_t		idx = p.rfind('.');
	if (idx != -1)
		return &p[idx];
	else
		return "";
}

bool		is_cgi(const string& extension)
{
	map<string, string>::iterator	it = Cgi::cgi_bin.find(extension);
	if (it == Cgi::cgi_bin.end())
		return false;
	return true;
}
/*
AUTH_TYPE
CONTENT_LENGTH
CONTENT_TYPE
GATEWAY_INTERFACE
PATH_INFO
PATH_TRANSLATED
QUERY_STRING
REMOTE_ADDR
REMOTE_IDENT
REMOTE_USER
REQUEST_METHOD
REQUEST_URI
SCRIPT_NAME
SERVER_NAME
SERVER_PORT
SERVER_PROTOCOL
SERVER_SOFTWARE
*/
char**		Client::make_meta_variable()
{
	char**	meta_var = new char*[18];
	string	meta_var_str[17];
	for (int i = 0 ; i < 16 ; i++)
	{
		meta_var_str[i].reserve(200);
	}
	meta_var_str[0] = string("AUTH_TYPE="			).append(location->auth_type);
	meta_var_str[1] = string("CONTENT_LENGTH="		).append(req.headers[CONTNET_LENGTH]);
	meta_var_str[2] = string("CONTENT_TYPE="		).append(req.headers[CONTNET_LENGTH]);
	meta_var_str[3] = string("GATEWAY_INTERFACE="	).append("CGI/1.1");
	meta_var_str[4] = string("PATH_INFO="			).append(req.path_info);
	meta_var_str[5] = string("PATH_TRANSLATED="		).append(path_translated);
	meta_var_str[6] = string("QUERY_STRING="		).append(req.query);
	meta_var_str[7] = string("REMOTE_ADDR="			).append(ft::addr_to_str(ft::hton(sock.s_addr.sin_addr.s_addr)));
	meta_var_str[8] = string("REMOTE_IDENT="		).append(req.headers[AUTHORIZATION]);
	meta_var_str[9] = string("REMOTE_USER="			).append("");
	meta_var_str[10] = string("REQUEST_METHOD="		).append(Method::method_strings[req.method]);
	meta_var_str[11] = string("REQUEST_URI="		).append(req.path_info);
	meta_var_str[12] = string("SCRIPT_NAME="		).append(path_translated);
	meta_var_str[13] = string("SERVER_NAME="		).append(server->name);
	meta_var_str[14] = string("SERVER_PORT="		).append(ft::itoa(server->port));
	meta_var_str[15] = string("SERVER_PROTOCOL="	).append(req.protocol);
	meta_var_str[16] = string("SERVER_SOFTWARE="	).append("Webserver42/1.0.0");
	for (int i = 0 ; i < 16 ; i++)
	{
		meta_var[i] = ft::strdup(meta_var[i], meta_var_str[i].c_str());
	}
	meta_var[17] = 0;
	return meta_var;
}


#include <stdio.h>
void		Client::process_put()
{
	switch (status_proc)
	{
		case STATUS_INIT:
		{
			#ifdef DBG
			cout << "- Method: PUT\n";
			cout << "- path: " << path_translated << endl;
			#endif

			if (ft::is_dir(path_translated.c_str()))
			{
				cout << "this is directory\n";
				cout << path_translated << endl;
				throw 404;
			}
			int		fd = open(path_translated.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
			{
				cout << "something wrong\n";
				perror("");
				throw 500 ;
			}
			stream_in.fd_out = fd;

			res.status_code = 201;
			stream_out << res.get_startline();
			stream_out << res.get_server();
			stream_out << "\r\n";

			if (req.headers[TRANSFER_ENCODING] == "chunked")
			{
				cout << "- transfer: chunked" << endl;
				status_proc = STATUS_RECV_CHUNKED_BODY;
				status_recv = STATUS_LEN;
			}
			else
			{
				cout << "- transfer: full" << endl;
				status_proc = STATUS_RECV_BODY;
				stream_in.pass_remain = ft::atoi_hex(req.headers[CONTNET_LENGTH]);
			}
			process_put();
			break;
		}
		case STATUS_RECV_BODY:
		{
			stream_in.pass(stream_in.pass_remain);
			if (stream_in.pass_remain == 0)
			{
				close(stream_in.fd_out);
				// status_proc = STATUS_RECV_DONE;
			}
			break;
		}
		case STATUS_RECV_CHUNKED_BODY:
		{
			while (42)
			{
				switch (status_recv)
				{
					case STATUS_LEN:
						if (!stream_in.get_line(line))
							return ;
						stream_in.pass_remain = ft::atoi_hex(line);
						cout << "- chunked size hex: " << line  << endl;
						cout << "- chunked size dec: " << stream_in.pass_remain << endl;
						if (stream_in.pass_remain > 1000000)
							exit(1);
						if (stream_in.pass_remain == 0 && !line.empty())
						{
							// status_proc = STATUS_RECV_DONE;
							cout << "recv end: " << stream_in.fd_out << endl;
							status = STATUS_SEND_MSG;
							close(stream_in.fd_out);
							return ;
						}
						else
						{
							status_recv = STATUS_BODY;
						}
						break ;
					case STATUS_BODY:
						cout << "- remain body size: " << stream_in.pass_remain << endl;
						cout << "- stream size " << stream_in.size() << endl;
						stream_in.pass(stream_in.pass_remain);
						if (!stream_in.pass_remain)
							status_recv = STATUS_NL;
						else
							return ;
						break ;
					case STATUS_NL:
						cout << "- nl\n";
						if (stream_in.get_line(line))
							status_recv = STATUS_LEN;
						else
							return ;
						break ;
				}
			}
		}
		case STATUS_RECV_DONE:
			break;
	}
}




void		Client::process_post()
{
	
	switch (status_proc)
	{
		case STATUS_INIT:
		{
			#ifdef DBG
			cout << "- Method: POST\n";
			cout << "- path: " << path_translated << endl;
			#endif

			if (ft::is_dir(path_translated.c_str()))
			{
				cout << "this is directory\n";
				throw 404;
			}

			meta_variables = make_meta_variable();
			cgi.init(path_translated.c_str(), meta_variables, stream_in.fd_out, sock.fd);
			cgi.start_cgi();
			delete[] meta_variables;

			if (req.headers[TRANSFER_ENCODING] == "chunked")
			{
				cout << "- transfer: chunked" << endl;
				status_proc = STATUS_RECV_CHUNKED_BODY;
				status_recv = STATUS_LEN;
			}
			else
			{
				cout << "- transfer: full" << endl;
				status_proc = STATUS_RECV_BODY;
				stream_in.pass_remain = ft::atoi_hex(req.headers[CONTNET_LENGTH]);
			}
			process_post();
			break;
		}
		case STATUS_RECV_BODY:
		{
			stream_in.pass(stream_in.pass_remain);
			if (stream_in.pass_remain == 0)
			{
				close(stream_in.fd_out);
				// status_proc = STATUS_RECV_DONE;
			}
			break;
		}
		case STATUS_RECV_CHUNKED_BODY:
		{
			while (42)
			{
				switch (status_recv)
				{
					case STATUS_LEN:
						if (!stream_in.get_line(line))
							return ;
						stream_in.pass_remain = ft::atoi_hex(line);
						cout << "- chunked size hex: " << line  << endl;
						cout << "- chunked size dec: " << stream_in.pass_remain << endl;
						if (stream_in.pass_remain > 1000000)
							exit(1);
						if (stream_in.pass_remain == 0 && !line.empty())
						{
							// status_proc = STATUS_RECV_DONE;
							cout << "recv end: " << stream_in.fd_out << endl;
							status = STATUS_SEND_MSG;
							close(stream_in.fd_out);
							return ;
						}
						else
						{
							status_recv = STATUS_BODY;
						}
						break ;
					case STATUS_BODY:
						cout << "- remain body size: " << stream_in.pass_remain << endl;
						cout << "- stream size " << stream_in.size() << endl;
						stream_in.pass(stream_in.pass_remain);
						if (!stream_in.pass_remain)
							status_recv = STATUS_NL;
						else
							return ;
						break ;
					case STATUS_NL:
						cout << "- nl\n";
						if (stream_in.get_line(line))
							status_recv = STATUS_LEN;
						else
							return ;
						break ;
				}
			}
		}
		case STATUS_RECV_DONE:

			while (42)
			{
				cgi.stream_out.get_line(line);
				if (line.empty())
					break ;
				req.set_header(line);
			}
			res.status_code = ft::atoi(req.headers[STATUS]);
			stream_out << res.get_startline();
			stream_out << res.get_server();
			stream_out << res.get_content_length(cgi.stream_out.size());
			stream_out << "\r\n";
			status = STATUS_SEND_MSG;
	}
}















// void		Client::process_post()
// {
// 	#ifdef DBG
// 	cout << "- Method: POST\n";
// 	cout << "- path: " << path_translated << endl;
// 	#endif

// 	string	extention = get_extention(path_translated);
// 	char**	meta_variable = make_meta_variable();
// 	cgi.init(path_translated.c_str(), meta_variable);
// 	cgi.start_cgi();
// 	cgi.fd_in = sock.fd;
// 	cgi.stream_out.fd_in = cgi.fd_out;

// 	if (req.headers[TRANSFER_ENCODING] == "chuncked")
// 		status_proc = STATUS_RECV_CHUNKED_BODY;
// 	else
// 		status_proc = STATUS_RECV_BODY;

// 	int		result;
// 	size_t	body_size;
// 	while (status_proc == STATUS_RECV_CHUNKED_BODY)
// 	{
// 		if (stream_in.pass_remain)
// 		{
// 			stream_in.pass(stream_in.pass_remain);
// 			if (stream_in.pass_remain)
// 				break;
// 		}
// 		result = stream_in.get_chr_token(line, '\n');
// 		if (result == true)
// 		{
// 			body_size = ft::atoi_hex(line);
// 			if (body_size == 0)
// 				status_proc = STATUS_RECV_DONE;
// 			stream_in.pass();
// 		}
// 	}

// 	if (status_proc == STATUS_RECV_BODY)
// 	{
// 		if (stream_in.pass_remain)
// 		{
// 			stream_in.pass(stream_in.pass_remain);
// 		}
// 		else
// 		{
// 			body_size = ft::atoi_hex(req.headers[CONTNET_LENGTH]);
// 			stream_in.pass(body_size);
// 		}
// 		if (stream_in.pass_remain == 0)
// 			status_proc = STATUS_RECV_DONE;
// 	}

// 	while (cgi.stream_out.fill(64000));






// 	#ifdef DBG
// 	cout << "::Response::\n\n";
// 	::write(1, stream_out.buffers.front().start, 
// 		stream_out.buffers.front().end - stream_out.buffers.front().start);
// 	cout << "\n---------------------------------------------------";
// 	cout << endl;
// 	#endif
	
// 	status = STATUS_SEND_MSG;
// }





























// void		Client::client_process(FdSet& r, FdSet& w)
// {
// 	if (r.get(sock.fd))
// 		buffer.is_readable = true;
// 	switch (status)
// 	{
// 		case RECV_START_LINE:
// 			cout << "recv startline\n";
// 			recv_start_line();
// 			if (status == RECV_START_LINE)
// 				break;
// 		case RECV_HEADER:
// 			cout << "recv header\n";
// 			recv_header();
// 			if (status == RECV_HEADER)
// 				break;
// 		case CHECK_MSG:
// 			set_location();		// 포트, 호스트, 로케이션 설정, 메서드, 권한 확인
// 			check_auth();
// 			check_method();
// 			status = PROC_MSG;
// 		case PROC_MSG:
// 			process_msg();
// 			if (status == PROC_MSG)
// 				break;
// 		case SEND_MSG:
// 			send_msg(w);
// 			if (status == SEND_MSG)
// 				break;
// 		case SEND_DONE:
// 			break ;
// 	}
// }

// //------------------------------------------------------------------------------

// void	Client::recv_start_line()
// {
// 	buffer.get_token(line, '\n');	// 첫 번째 개행을 만나기 전이라면 그대로 리턴
// 	if (!buffer.is_token_complete)
// 	{
// 		return ;
// 	}
// 	req.set_start_line(line);		// 첫 번째 개행을 만났다면 헤더를 구문분석
// 	status = RECV_HEADER;			// 시작줄 구문분석 완료 후 상태플래그 변경
// }

// //------------------------------------------------------------------------------

// void	Client::recv_header()
// {
// 	buffer.get_token(line, '\n');
// 	if (!line.empty() && *--line.end() == '\r')
// 	{
// 		line.erase(--line.end());
// 	}
// 	if (!buffer.is_token_complete)
// 	{
// 		return ;
// 	}
// 	if (line == "")
// 	{
// 		status = PROC_MSG;
// 		return ;
// 	}
// 	req.set_header(line);	// Http 한 줄 구문분석 (키/값 분리)
// 	recv_header();
// }

// //------------------------------------------------------------------------------

// void		Client::set_location()
// {
// 	//호스트 헤더에서 호스트 추출
// 	string&				host = req.headers[HOST];
// 	char*				port_sep = ft::strchr(host.c_str(), ':');
// 	if (port_sep)
// 		host.erase(host.begin() + (port_sep - &host[0]), host.end());

// 	// 서버에서 호스트 검색
// 	iterator_server		it_server = servers.find(host);
// 	if (it_server == servers.end())
// 		throw 404;
// 	server = &it_server->second;

// 	// 메시지에서 로케이션 추출
// 	string			http_location_name;
// 	req.get_location_name(http_location_name);

// 	// 로케이션 설정
// 	iterator_location	it_location = server->locations.find(http_location_name);
// 	if (it_location == server->locations.end())
// 		throw 404;
// 	else
// 		location = &it_location->second;

// 	// 경로 치환
// 	replace_location();
// }

// //------------------------------------------------------------------------------

// void		Client::replace_location()
// {
// 	path = server->root;
// 	path += "/";
// 	path += location->root;

// 	if (req.path.size() == 1)
// 	{
// 		if (req.path.front() != "..")
// 			throw 403;
// 		path += req.path.front();
// 		return ;
// 	}
// 	req.path.pop_front();
// 	for (list<string>::iterator it = req.path.begin() ; it != req.path.end() ; ++it)
// 	{
// 		if (*it == "..")
// 		{
// 			if (req.path.empty())
// 				throw 403;
// 			--it;
// 			it = req.path.erase(it);
// 			it = req.path.erase(it);
// 			if (req.path.empty())
// 				throw 403;
// 		}
// 	}
// 	for (list<string>::iterator it = req.path.begin() ; it != req.path.end() ; ++it)
// 	{
// 		path += "/";
// 		path += *it;
// 	}
// }

// //------------------------------------------------------------------------------

// void		Client::check_auth()
// {
// 	string&					line = req.headers[AUTHORIZATION];
// 	string::const_iterator	it = line.begin();
// 	string					client_auth_type;
// 	string					client_auth;
// 	ft::get_chr_token(line, it, client_auth_type, ' ');
// 	if (location->auth_type != client_auth_type)
// 		throw 401;	// TODO: bad request or unauthorized?
// 	ft::get_chr_token(line, it, client_auth, ' ');
// 	if (location->auth != client_auth)
// 		throw 401;	// TODO: bad request or unauthorized?
// }

// //------------------------------------------------------------------------------

// void		Client::check_method()
// {
// 	if (location->method & (1 << req.method)) // ok
// 		return ;
// 	else
// 		throw 405; // method not allowed
// }


// //------------------------------------------------------------------------------
// void	Client::process_msg()
// {
// 	switch (req.method)
// 	{
// 		case GET:
// 			process_get();
// 			break;
// 		case PUT:
// 			process_put();
// 			break;
// 		case POST:
// 			process_post();
// 			break;
// 		case OPTIONS:
// 			process_options();
// 			break;
// 		case DELETE:
// 			process_delete();
// 			break;
// 		default:
// 			break;
// 	}
// }

// //------------------------------------------------------------------------------

// void	Client::process_get()
// {
// 	size_t	filesize = ft::file_size(path.c_str());
// 	msg.reserve(8000 + filesize);
// 	res.status_code = 200;
// 	msg += res.get_startline();
// 	msg += res.get_content_length(filesize);
// 	msg += res.get_server();
// 	msg += "\r\n";

// 	int		fd_get = open(path.c_str(), O_RDONLY);
// 	if (fd_get < 0)
// 		throw 404; // not found
// 	ssize_t	len = read(fd_get, msg.end().base(), filesize);
// 	if (len < 0)
// 		throw 500;
// 	status = SEND_MSG;
// }

// //------------------------------------------------------------------------------

// void	Client::process_put()
// {
// 	msg.reserve(10000);
// 	// msg += res.get_startline(200);

// }

// //------------------------------------------------------------------------------

// void	Client::process_post()
// {
// 	msg.reserve(10000);
// 	// msg += res.get_startline(200);

// }

// //------------------------------------------------------------------------------

// void	Client::process_options()
// {
// 	msg.reserve(10000);
// 	// msg += res.get_startline(200);

// }

// //------------------------------------------------------------------------------

// void	Client::process_delete()
// {
// 	msg.reserve(10000);
// 	// msg += res.get_startline(200);

// }

// void	Client::send_get()
// {

// }













// // //------------------------------------------------------------------------------

// // void	Client::proc_cgi()
// // {
// // 	char**	meta_variable = make_meta_variable();
// // 	cgi.init(location->cgi.c_str(), meta_variable);
// // 	cgi.start_cgi();
// // }

// // char**	Client::make_meta_variable()
// // {
// // 	char**		result = new char*[10];
// // 	(void)result;
// // 	return 0;
// // }

// // //------------------------------------------------------------------------------

// // void	Client::recv_body(size_t len)
// // {
// // 	cgi.check_exit();
// // 	if (cgi.is_exit)
// // 	{
// // 		status_proc = END_CGI;
// // 		return ;
// // 	}
// // 	if (buffer.write_request)
// // 		buffer.write(buffer.write_request, cgi.fd_out);
// // 	else
// // 		buffer.write(len, cgi.fd_out);
// // }

// // //------------------------------------------------------------------------------

// // void	Client::recv_body_chunked()
// // {
// // 	cgi.check_exit();
// // 	if (cgi.is_exit)
// // 	{
// // 		status_proc = END_CGI;
// // 		return ;
// // 	}
// // 	if (!buffer.write_request)
// // 	{
// // 		buffer.get_token(line, '\n');
// // 		if (!buffer.is_token_complete)
// // 		{
// // 			buffer.read_request = true;
// // 			return ;
// // 		}
// // 		size_t	len = atol(line.c_str());
// // 		buffer.write(len, cgi.fd_out);
// // 	}
// // 	else
// // 	{
// // 		buffer.write(buffer.write_request, cgi.fd_out);
// // 	}
// // }

// // //------------------------------------------------------------------------------

// // void	Client::terminate_cgi()
// // {
// // 	cgi.terminate();
// // }

// // //------------------------------------------------------------------------------

// // void	Client::make_msg()
// // {
// // 	std::string	header;
// // 	switch (req.method)
// // 	{
// // 	case GET:
// // 		header += "HTTP/1.1 200 ";
// // 		header += HttpRes::status_code_map[200];
// // 		header += "\r\n";
// // 		header += "Content-Length: ";
// // 		header += ft::itoa(res.body.size());
// // 		header += "\r\n";
// // 		res.body.insert(res.body.begin(), header.begin(), header.end());
// // 		break;
	
// // 	default:
// // 		break;
// // 	}
// // }

// //------------------------------------------------------------------------------

// void	Client::send_msg(FdSet& w)
// {
// 	write(sock.fd, msg.begin().base(), msg.length());
// 	status = SEND_DONE;
// }
