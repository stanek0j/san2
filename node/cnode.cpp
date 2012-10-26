
#include "cnode.hpp"
#include "utils/log.h"

namespace San2 { namespace Node {

CNode::CNode(unsigned int inputQueueMaxSize) :
	m_inputQueue(inputQueueMaxSize),
	m_duration(3)
{

}

void CNode::run()
{
	// there should be capsule executor/worker implementation
	
	// there should be a router implementation
	
	// however there is only simple algorithm which checks
	// if the destination SanAddress matches one of our 
	// direct peers SanAddress and sends the capsule that way.
	
	std::shared_ptr<San2::Network::CCapsule> capsule;
	
	while(1)
	{
		m_inputQueue.pop<int>(&capsule, this, m_duration);
		FILE_LOG(logDEBUG4) << "CNode::run()::pop() SUCCESS ######";
		
		/*
		San2::Network::SanAddress adr = capsule.getDestinationAddress();
	
		bool rval;
	
		// this is dangerous, race-condition might occur in future
		
		{
			std::locked_guard<std::mutex> lock(m_mutexInterfaces);
			std::for_each(m_interface.begin(), m_interface.end(), [&adr, &rval](std::shared_ptr<San2::Network::CNetInterface> iface){if (adr == iface.getPeerAddress(); rval = iface.sendCapsule())})
		}
		*/
		
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

// do not confuse with routing, just puts capsule into the inputQueue
bool CNode::injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule)
{
	return m_inputQueue.push(capsule);
}

template <class Rep, class Period>
bool CNode::injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur)
{
	return m_inputQueue.push(capsule, this, dur);
}

template <class Rep, class Period>
bool CNode::tryInjectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur)
{
	return m_inputQueue.try_push(capsule, this, dur);
}


}} // ns
