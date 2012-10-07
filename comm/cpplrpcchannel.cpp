
#include "cpplrpcchannel.hpp"
#include "stream/clenvalueseparator.hpp"

namespace San2 { namespace Comm {

CpplRpcChannel::CpplRpcChannel(San2::Cppl::BufferProcessor *bp) :
	m_bp(bp),
	m_rw(2048, bp)
{
	m_separator = new San2::Stream::CLenValueSeparator(m_rw);
}

bool CpplRpcChannel::sendDatagram(const San2::Utils::bytes &data)
{
	// TODO: Timeout (+ thread term?)
	return m_separator->writeMessage(data);
}

bool CpplRpcChannel::recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec)
{
	// TODO: Timeout (+ thread term?)
	return m_separator->readMessage(out);
}

}} // ns
