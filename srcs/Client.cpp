#include "Client.hpp"

void		Client::manage_client(bool is_buffer)
{
	if (is_buffer)
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
		case PROC_CGI:
			proc_cgi();
			if (status == PROC_CGI)
				break;
		case RECV_BODY:
			recv_body();
			if (status == RECV_BODY)
				break;
		case RECV_BODY_CHUNKED:
			recv_body();
			if (status == RECV_BODY)
				break;
		case END_CGI:
			recv_header();
			if (status == END_CGI)
				break;
		case MAKE_MSG:
			recv_header();
			if (status == MAKE_MSG)
				break;
		case SEND_MSG:
			recv_header();
			if (status == SEND_MSG)
				break;
		default:
			break;
	}
}

void	Client::read_buffer()
{
	buffer.read_buffer();
}

void	Client::recv_start_line()
{
	// 첫 번째 개행을 만나기 전이라면 그대로 리턴
	if (!buffer.get_token(line, '\n'))
		return ;
	// 첫 번째 개행을 만났다면 헤더를 구문분석 한다
	// 시작줄 구문분석 완료 후 상태플래그 변경
	status = RECV_HEADER;
}

void	Client::recv_header()
{
	if (!buffer.get_token(line, '\n'))
		return ;
	if (line == "" || line == "\r")
	{
		status = PROC_CGI;
		return ;
	}
	// Http 한 줄 구문분석
}

void	Client::proc_cgi()
{
	// cgi.init_cgi();
	cgi.start_cgi();
}

void	Client::recv_body()
{
	cgi.connect();
	buffer.write();
	cgi.disconnect();
}

void	Client::recv_chunked_body()
{

}

void	Client::terminate_cgi()
{

}

void	Client::make_msg()
{

}

void	Client::send_msg()
{

}
