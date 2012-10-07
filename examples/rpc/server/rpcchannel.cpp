
#include "rpcchannel.hpp"


RpcChannel::RpcChannel(San2::Cppl::BufferProcessor *bp) :
	m_bp(bp)
{
	
}

bool RpcChannel::sendDatagram(const San2::Utils::bytes &data)
{
	
}

bool RpcChannel::recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec)
{
	
}
