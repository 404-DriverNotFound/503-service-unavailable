#include "../includes/Client.hpp"

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

void		Client::init(int fd)
{
	sock.accept(fd);
	status = RECV_START_LINE;
}

//------------------------------------------------------------------------------

void		client_process(FdSet& r, FdSet& w)
{
	if (r.get(sock.fd))
		stream_in.fill(BUFFER_SIZE);
	if (w.get(sock.fd))
		stream_out.write(sock.fd);
	
}















































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
