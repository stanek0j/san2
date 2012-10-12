
#include "tcprpcchannel.hpp"
#include "stream/clenvalueseparator.hpp"

namespace San2 { namespace Comm {

TcpRpcChannel::TcpRpcChannel(San2::Tcp::CITcpBuffer *bp) :
	m_bp(bp),
	m_rw(2048, bp)
{
	m_separator = new San2::Stream::CLenValueSeparator(m_rw);
}

bool TcpRpcChannel::sendDatagram(const San2::Utils::bytes &data)
{
	// TODO: Timeout (+ thread term?)
	return m_separator->writeMessage(data);
}

bool TcpRpcChannel::recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec)
{
	// TODO: Timeout (+ thread term?)
	return m_separator->readMessage(out);
}

}} // ns
