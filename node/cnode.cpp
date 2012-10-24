
#include "cnode.hpp"

namespace San2 { namespace Node {

CNode::CNode(unsigned int inputQueueMaxSize) :
	m_inputQueue(inputQueueMaxSize),
	m_duration(3)
{

}

void CNode::run()
{
	std::shared_ptr<San2::Network::CCapsule> capsule;
	
	while(1)
	{
		m_inputQueue.pop<int>(&capsule, this, m_duration);
		printf("CNode::run()::pop() SUCCESS\n");
	}
}

int CNode::addInterface(std::shared_ptr<San2::Network::CNetInterface> iface)
{
	iface->up();
	m_interfaces.insert(iface);
	return 0;
}


San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& CNode::getInputQueue()
{
	return m_inputQueue;
}

}} // ns
