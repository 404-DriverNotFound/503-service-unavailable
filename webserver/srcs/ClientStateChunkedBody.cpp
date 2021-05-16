#include "../includes/ClientStateChunkedBody.hpp"

ClientStateChunkedBody::ClientStateChunkedBody()
{
}

ClientStateChunkedBody::~ClientStateChunkedBody()
{
}

ClientState* ClientStateChunkedBody::action(Client& ref)
{
	std::cout << "ChunkedBody" << std::endl;
	return (ClientState*)NULL;
}
