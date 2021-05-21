#include "../includes/StateLoadBody.hpp"
#include "../includes/StateWait.hpp"
#include "../includes/StateDone.hpp"

StateLoadBody::StateLoadBody()
{
}

StateLoadBody::~StateLoadBody()
{
}

State*	StateLoadBody::action(Client& ref)
{
	ssize_t		len_read;
	HttpRes&	res = ref.get_httpres();
	if (res.get_file_size() <= res.get_stream().get_last_capacity())
		len_read = res.get_stream().fill(res.get_file_size());
	else
		len_read = res.get_stream().fill(0xffffffff);
	if (len_read == 0)
		return waiting;
	return loadbody;
}