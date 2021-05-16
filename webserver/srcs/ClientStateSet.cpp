#include "../includes/ClientStateSet.hpp"
#include "../includes/ClientStateBody.hpp"
#include "../includes/ClientStateChunkedBody.hpp"

ClientStateSet::ClientStateSet()
{
}

ClientStateSet::~ClientStateSet()
{
}

ClientState*	ClientStateSet::action(Client &ref)
{
	std::cout << "Set" << std::endl;
	if (true)
		return	body;
	else
		return	chunkedbody;
}
