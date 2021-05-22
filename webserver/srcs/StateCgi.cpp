#include "../includes/StateCgi.hpp"
#include "../includes/StateLoadBody.hpp"

StateCgi::StateCgi()
{
}

StateCgi::~StateCgi()
{
}

State*	StateCgi::action(Client& ref)
{
	// cout << __func__ << ": cgi" << endl;
	if (ref.check_cgi_exit())
	{
		terminate_cgi(ref);
		return loadbody;
	}
	return cgi;
}

void	StateCgi::terminate_cgi(Client& ref)
{
	// cout << __func__ << endl;
	HttpRes&	res = ref.get_httpres();
	Stream		stream(8000, res.get_file_fd());

	stream.fill(200);
	
	size_t	cgi_header_size = stream.size();
	string	line;

	while (stream.get_line(line))
	{
		if (line.empty())
			break ;
		res.set_header(line);
	}
	cgi_header_size -= stream.size();

	size_t	content_length = ft::file_size(res.get_file_name().c_str()) - cgi_header_size;

	lseek(stream.get_fd_in(), cgi_header_size, SEEK_SET);
	
	string	status_code_str = res.get_header("STATUS");

	res.set_status_code(ft::atoi(Tokenizer(status_code_str).chr(' ')));
	res.get_stream() << res.get_start_line();
	res.get_stream() << "Content-Length: " << ft::itoa(content_length) << "\r\n";
	res.get_stream() << "\r\n";
	res.set_len_msg(res.get_stream().size() + content_length);
}