#pragma once

#include <memory>
#include "nettypedef.hpp"
#include "utils/cthread.hpp"
#include "ccapsule.hpp"

namespace San2
{
	namespace Network
	{
		// It is up to the implementation to satisfy all the requirements.
		
		// Receiving is performed by passing InputQueue reference to derived base contructor
		// InputQueue should be obtained from CNode::getInputQueue();
		
		class CNetInterface
		{
		public:
			// NON-BLOCKING
			// starts the interface			
			virtual void up()=0; 
			
			// NON-BLOCKING, return value (true - queue was not full, false - queue full, drop occured)
			// NOTE: should likely put the capsule to its output queue, if queue is full then drop
			virtual bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr)=0;
			
			// NON-BLOCKING, MULTI-READ THREAD SAFE
			// returns pointer to the underlying thread, NULL if not availible
			virtual San2::Utils::CThread* getThread()=0; // might be replaced by other function in future releases
			
			// NON-BLOCKING, MULTI-READ THREAD SAFE
			// returns DefaultSanAddress if not availible
			virtual SanAddress getPeerAddress()=0;
			
			// NON-BLOCKING, MULTI-READ THREAD SAFE
			virtual SanAddress getInterfaceAddress()=0;			
		};
	}
}
