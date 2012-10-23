
#include "cnode.hpp"

namespace San2 { namespace Node {

int CNode::startNode()
{
	
}

int CNode::addInterface(std::shared_ptr<San2::Network::CInputInterface> iface)
{
	iface->up();
	m_inputInterfaces.insert(iface);
}

int CNode::addInterface(std::shared_ptr<San2::Network::COutputInterface> iface)
{
	iface->up();
	m_outputInterfaces.insert(iface);
}

San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& getInputQueue()
{
	return m_inputQueue;
}

}} // ns
