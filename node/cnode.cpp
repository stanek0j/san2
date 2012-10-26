
#include "cnode.hpp"

namespace San2 { namespace Node {

CNode::CNode(unsigned int inputQueueMaxSize) :
	m_inputQueue(inputQueueMaxSize),
	m_duration(3)
{

}

void CNode::run()
{
	// there should be capsule executor/worker implementation
	
	std::shared_ptr<San2::Network::CCapsule> capsule;
	
	while(1)
	{
		m_inputQueue.pop<int>(&capsule, this, m_duration);
		printf("CNode::run()::pop() SUCCESS\n");
	}
}

int CNode::addInterface(std::shared_ptr<San2::Network::CNetInterface> iface)
{
	std::lock_guard<std::mutex> lock(m_mutexInterfaces);
	iface->up();
	m_interfaces.insert(iface);
	return 0;
}


San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& CNode::getInputQueue()
{
	return m_inputQueue;
}

/*
// TRUE - capsule MIGHT be send (depends on interface behaviour)
// FALSE - capsule was definitely not send
bool CNode::sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule)
{
	
	// there should be a router implementation
	
	// however there is only simple algorithm which checks
	// if the destination SanAddress matches one of our 
	// direct peers SanAddress and sends the capsule that way.
	
	San2::Network::SanAddress adr = capsule.getDestinationAddress();
	
	bool rval;
	
	// this is dangerous, race-condition might occur in future
	std::locked_guard<std::mutex> lock(m_mutexInterfaces);
	std::for_each(m_interface.begin(), m_interface.end(), [&adr, &rval](std::shared_ptr<San2::Network::CNetInterface> iface){if (adr == iface.getPeerAddress(); rval = iface.sendCapsule())})
	
}
*/

}} // ns
