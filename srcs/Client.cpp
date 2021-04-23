#include "Client.hpp"

//------------------------------------------------------------------------------

			Client::Client(int fd, std::map<std::string, Server>& servers)
: sock(fd), buffer(fd), status(RECV_START_LINE), servers(servers)
{}

			Client::~Client()
{}

//------------------------------------------------------------------------------

void		Client::client_process(FdSet& r, FdSet& w)
{
	if (buffer.read_request && r.get(sock.fd))
	{
		read_buffer();
	}
	switch (status)
	{
		case RECV_START_LINE:
			recv_start_line();
			if (status == RECV_START_LINE)
				break;
		case RECV_HEADER:
			recv_header();
			if (status == RECV_HEADER)
				break;
		case PROC_MSG:
			set_location();		// 포트, 호스트, 로케이션 설정, 메서드, 권한 확인
			check_auth();
			check_method();
			proc_cgi();
			status = RECV_BODY;
		case RECV_BODY:
			recv_body(0);
			if (status == RECV_BODY)
				break;
		case RECV_BODY_CHUNKED:
			recv_body_chunked();	// cgi 종료상태도 체크
			if (status == RECV_BODY)
				break;
		case END_CGI:
			terminate_cgi();
			if (status == END_CGI)
				break;
		case MAKE_MSG:
			make_msg();
		case SEND_MSG:
			send_msg(w);
			if (status == SEND_MSG)
				break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------

void	Client::read_buffer()
{	buffer.read_buffer();	}

//------------------------------------------------------------------------------

void	Client::recv_start_line()
{
	buffer.get_token(line, '\n');	// 첫 번째 개행을 만나기 전이라면 그대로 리턴
	if (!buffer.is_token_complete)
	{
		buffer.read_request = true;
		return ;
	}
	req.set_start_line(line);		// 첫 번째 개행을 만났다면 헤더를 구문분석
	status = RECV_HEADER;			// 시작줄 구문분석 완료 후 상태플래그 변경
}

//------------------------------------------------------------------------------

void	Client::recv_header()
{
	buffer.get_token(line, '\n');
	if (!buffer.is_token_complete)
	{
		buffer.read_request = true;
		return ;
	}
	if (line == "" || line == "\r")
	{
		status = PROC_MSG;
		return ;
	}
	req.set_headers(line);		// Http 한 줄 구문분석 (키/값 분리)
}

//------------------------------------------------------------------------------

void		Client::set_location()
{
	typedef std::map<std::string, std::string>::iterator	host_iterator;
	typedef std::map<std::string, Server>::iterator		server_iterator;
	typedef std::map<std::string, Location>::iterator	location_iterator;

	//호스트 헤더에서 호스트 추출
	host_iterator		host_it = req.headers.find("host");
	if (host_it == req.headers.end())
		throw 400;	// 아마도
	const std::string&		host = host_it->second;

	// 로케이션 추출
	std::string			http_location_name;
	req.get_location_name(http_location_name);

	// 서버에서 호스트 검색
	server_iterator		server_it = servers.find(host);
	if (server_it == servers.end())
		throw 404;
	server = &server_it->second;

	// 로케이션
	location_iterator	location_it = server->location.find(http_location_name);
	if (location_it == server->location.end())
	{
		throw 404;
	}
	else
	{
		location = &location_it->second;
	}

	//------------------------------



	server_iterator 	server_it = servers.begin();
	server_iterator 	server_end = servers.end();
	header_iterator		header_it;
	while (server_it != server_end)
	{
		// if ((header_it = req.headers.find("host")) == req.headers.end())
		// 	throw 401;
		if (req.headers["host"] == server_it->first)
		{
			server = &(*server_it).second;
			location_iterator 	location_it = server->location.begin();
			location_iterator 	location_end = server->location.end();
			while (location_it != location_end)
			{
				if (location_it->location == http_location_name)
				{
					location = &(*location_it);
					replace_location();
					return ;
				}
				++location_it;
			}
		}
		++server_it;
	}
	throw 404;
}

void		Client::replace_location()
{
	path = server->root + location->root + ft::strchr(&req.url[0], '/');
}

void		Client::check_auth()
{
	if (location->auth.empty())
		return ;
	header_iterator		it_header = req.headers.find("authorization");
	if (it_header == req.headers.end())
		return ;
	else
		throw 401;	// not authorized
	return ;
}

void		Client::check_method()
{
	if (location->method & (1 << req.method)) // ok
		return ;
	else
		throw 405; // method not allowed
}

//------------------------------------------------------------------------------

void	Client::proc_cgi()
{
	char**	meta_variable = make_meta_variable();
	cgi.init(location->cgi.c_str(), meta_variable);
	cgi.start_cgi();
}

char**	Client::make_meta_variable()
{
	char**		result = new char*[10];
	(void)result;
	return 0;
}

//------------------------------------------------------------------------------

void	Client::recv_body(size_t len)
{
	cgi.check_exit();
	if (cgi.is_exit)
	{
		status = END_CGI;
		return ;
	}
	if (buffer.write_request)
		buffer.write(buffer.write_request, cgi.fd_out);
	else
		buffer.write(len, cgi.fd_out);
}

//------------------------------------------------------------------------------

void	Client::recv_body_chunked()
{
	cgi.check_exit();
	if (cgi.is_exit)
	{
		status = END_CGI;
		return ;
	}
	if (!buffer.write_request)
	{
		buffer.get_token(line, '\n');
		if (!buffer.is_token_complete)
		{
			buffer.read_request = true;
			return ;
		}
		size_t	len = atol(line.c_str());
		buffer.write(len, cgi.fd_out);
	}
	else
	{
		buffer.write(buffer.write_request, cgi.fd_out);
	}
}

//------------------------------------------------------------------------------

void	Client::terminate_cgi()
{
	cgi.terminate();
}

//------------------------------------------------------------------------------

void	Client::make_msg()
{
	std::string	header;
	switch (req.method)
	{
	case GET:
		header += "HTTP/1.1 200 ";
		header += HttpRes::status_code_map[200];
		header += "\r\n";
		header += "Content-Length: ";
		header += ft::itoa(res.body.size());
		header += "\r\n";
		res.body.insert(res.body.begin(), header.begin(), header.end());
		break;
	
	default:
		break;
	}
}

//------------------------------------------------------------------------------

void	Client::send_msg(FdSet& w)
{
	std::vector<uint8_t>	body(res.body.begin(), res.body.end());
	if (w.get(sock.fd))
		write(sock.fd, &body[0], body.size());
}
