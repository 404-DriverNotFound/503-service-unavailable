#include "Client.hpp"

//------------------------------------------------------------------------------

			Client::Client(int fd, std::vector<Server>& vec_server)
: sock(fd), buffer(fd), status(RECV_START_LINE), vec_server(vec_server)
{}

//------------------------------------------------------------------------------

void		Client::client_process(FdSet& r, FdSet& w)
{
	(void)w; // 아직 사용안햠
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
			send_msg();
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
	typedef std::vector<Server>::iterator		server_iterator;
	typedef std::vector<Location>::iterator		location_iterator;

	std::string			http_location_name;
	req.get_location_name(http_location_name);
	server_iterator 	server_it = vec_server.begin();
	server_iterator 	server_end = vec_server.end();
	while (server_it != server_end)
	{
		if (req.headers["host"] == server_it->server_name)
		{
			server = &(*server_it);
			location_iterator 	location_it = server->location.begin();
			location_iterator 	location_end = server->location.end();
			while (location_it != location_end)
			{
				if (location_it->location == http_location_name)
				{
					location = &(*location_it);
					return ;
				}
				++location_it;
			}
		}
		++server_it;
	}
	throw 404;
}

void		Client::check_auth()
{

}

//------------------------------------------------------------------------------

void	Client::proc_cgi()
{
	// cgi.init_cgi();
	cgi.start_cgi();
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

}

//------------------------------------------------------------------------------

void	Client::send_msg()
{

}
