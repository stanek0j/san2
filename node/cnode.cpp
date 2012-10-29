#include <algorithm>
#include <assert.h>
#include <stdlib.h>

#include "cnode.hpp"
#include "utils/log.h"

namespace San2 { namespace Node {

CNode::CNode(unsigned int inputQueueMaxSize, std::string nodeName) :
	m_inputQueue(inputQueueMaxSize),
	m_duration(3),
	m_nodeName(nodeName)
{

}

void CNode::run()
{
	// there should be capsule executor/worker implementation
	
	// there should be a router implementation
	
	
	
	std::shared_ptr<San2::Network::CCapsule> capsule;
	
	while(1)
	{
		bool rval;
		
		FILE_LOG(logDEBUG4) << "CNode::run()::pop(): waiting for pop";
		m_inputQueue.pop<int>(&capsule, this, m_duration);
		FILE_LOG(logDEBUG4) << "CNode::run()::pop(): gotCapsule ######";
		
	
		// check if we reached destination
		rval = false;
		{
			std::lock_guard<std::mutex> lock(m_mutexInterfaces);
			
			std::for_each(m_interfaces.begin(), m_interfaces.end(), 
				[&rval, capsule](std::shared_ptr<San2::Network::CNetInterface> iface) 
			{
				/*
				FILE_LOG(logDEBUG4) << "KK4";
				
				printf("capsule->getDestinationAddress(): ");
				San2::Network::SanAddress tmp = capsule->getDestinationAddress();
				for (unsigned int v = 0 ; v < San2::Network::sanAddressSize ; ++v ) printf("%02X", tmp[v]);
				printf("\n");
				
				printf("iface->getInterfaceAddress():");
				tmp = iface->getInterfaceAddress();
				for (unsigned int v = 0 ; v < San2::Network::sanAddressSize ; ++v ) printf("%02X", tmp[v]);
				printf("\n");
				printf("\n");
				*/
				
				if (capsule->getDestinationAddress() == iface->getInterfaceAddress()){rval = true;}
			});
		}
		
		
		
		if (rval == true)
		{
			FILE_LOG(logDEBUG4) << "CNode::run():: capsule reached its final destination ######";
			continue;
		}
	
		// very simple routing algorithm which checks
		// if the destination SanAddress matches one of our 
		// direct peers SanAddress and sends the capsule that way.	
		rval = false;
		{
			std::lock_guard<std::mutex> lock(m_mutexInterfaces);
			
			// FILE_LOG(logDEBUG4) << "KK3";
			std::for_each(m_interfaces.begin(), m_interfaces.end(), [&rval, capsule, this](std::shared_ptr<San2::Network::CNetInterface> iface)
			{
				
				/*
				FILE_LOG(logDEBUG4) << "KK4";
				
				printf("capsule->getDestinationAddress(): ");
				San2::Network::SanAddress tmp = capsule->getDestinationAddress();
				for (unsigned int v = 0 ; v < San2::Network::sanAddressSize ; ++v ) printf("%02X", tmp[v]);
				printf("\n");
				
				printf("iface->getPeerAddress(): ");
				tmp = iface->getPeerAddress();
				for (unsigned int v = 0 ; v < San2::Network::sanAddressSize ; ++v ) printf("%02X", tmp[v]);
				printf("\n");
				printf("\n");
				*/
				
				if (capsule->getDestinationAddress() == iface->getPeerAddress()) 
				{
					rval = iface->sendCapsule(capsule, this); 
					if (!rval) 
					{
						FILE_LOG(logDEBUG4) << "CNode::run(): iface->sendCapsule failed";
					}
				}
			});
		}
		
		if (rval == true) 
		{
			FILE_LOG(logDEBUG4) << "CNode::run():: rerouting capsule";
		}
		
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
	assert(capsule != NULL);
	return m_inputQueue.push(capsule);
}

template <class Rep, class Period>
bool CNode::injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur)
{
	assert(capsule != NULL);
	return m_inputQueue.push(capsule, this, dur);
}

template <class Rep, class Period>
bool CNode::tryInjectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur)
{
	assert(capsule != NULL);
	return m_inputQueue.try_push(capsule, this, dur);
}

std::string CNode::getNodeName() const
{
	return m_nodeName;
}

}} // ns
