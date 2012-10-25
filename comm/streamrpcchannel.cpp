
#include "streamrpcchannel.hpp"

namespace San2 { namespace Comm {

StreamRpcChannel::StreamRpcChannel(San2::Stream::CIStreamRW &rw) :
	m_separator(rw)
{
	
}

bool StreamRpcChannel::sendDatagram(const San2::Utils::bytes &data)
{
	// TODO: Timeout (+ thread term?)
	return m_separator.writeMessage(data);
}

bool StreamRpcChannel::recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec)
{
	// TODO: Timeout (+ thread term?)
	return m_separator.readMessage(out);
}

}} // ns
