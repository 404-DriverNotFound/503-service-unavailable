#include "../includes/ClientStateStartLine.hpp"

void	ClientStateStartLine::done(Client& ref)
{
	ref.getReq().set_start_line(ref.getReq().);
}