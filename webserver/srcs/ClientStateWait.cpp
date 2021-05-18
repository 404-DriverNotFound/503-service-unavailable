#include "../includes/ClientStateWait.hpp"
#include "../includes/ClientStateDone.hpp"

ClientStateWait::ClientStateWait()
{
}

ClientStateWait::~ClientStateWait()
{
}

ClientState*	ClientStateWait::action(Client& ref)
{
	cout << "Wait : " <<  __func__ << endl;
	cout << ref.get_httpres().get_file_fd() << endl;
	cout << ref.get_httpres().get_file_name() << endl;
	cout << ref.get_httpres().get_stream().get_fd_in() << endl;
	if (ref.get_httpres().get_stream().fill(0xffff) == 0)
		return done;
	return waiting;
}