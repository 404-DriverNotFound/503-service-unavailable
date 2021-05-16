#include "../includes/HttpRes.hpp"

HttpRes::HttpRes()
{
}

HttpRes::~HttpRes()
{
}

void	HttpRes::set_status_code(int code)
{
	_status_code = code;
}
