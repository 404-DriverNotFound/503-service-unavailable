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
	cout << std::setw(13) << "LoadBody : " <<  __func__ << endl;
	cout << std::setw(13) << "file fd : " << ref.get_httpres().get_file_fd() << endl;
	cout << std::setw(13) << "file name : " << ref.get_httpres().get_file_name() << endl;
	cout << std::setw(13) << "fd in : " << ref.get_httpres().get_stream().get_fd_in() << endl;
	cout << std::setw(13) << "fd out : " << ref.get_httpres().get_stream().get_fd_out() << endl;
	if (ref.get_httpres().get_stream().fill(0xffff) == 0)
	{
		return waiting;
	}
	return loadbody;
}