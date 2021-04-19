#include "Client.hpp"

//------------------------------------------------------------------------------

			Client::Client(int fd)
: sock(fd), buffer(fd), status(RECV_START_LINE)
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
		case PROC_CGI:
			proc_cgi();
			if (status == PROC_CGI)
				break;
		case RECV_BODY:
			recv_body(0);
			if (status == RECV_BODY)
				break;
		case RECV_BODY_CHUNKED:
			recv_body_chunked();
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
		status = PROC_CGI;
		return ;
	}
	req.set_headers(line);		// Http 한 줄 구문분석 (키/값 분리)
}

//------------------------------------------------------------------------------

// void	Client::set_server(std::vector<Config>& configs)
// {
// 	std::vector<Config>::iterator	it = configs.begin();
// 	std::vector<Config>::iterator	end = configs.end();

// 	while (it != end)
// 	{
// 		if (it->root == req.path)
// 	}
// }

//------------------------------------------------------------------------------

void	Client::proc_cgi()
{
	// cgi.init_cgi();
	cgi.start_cgi();
}

//------------------------------------------------------------------------------

void	Client::recv_body(size_t len)
{
	if (buffer.write_request)
		buffer.write(buffer.write_request, cgi.fd_out);
	else
		buffer.write(len, cgi.fd_out);
}

//------------------------------------------------------------------------------

void	Client::recv_chunked_body()
{
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

}

//------------------------------------------------------------------------------

void	Client::make_msg()
{

}

//------------------------------------------------------------------------------

void	Client::send_msg()
{

}
