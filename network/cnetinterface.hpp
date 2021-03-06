#pragma once

#include <atomic>
#include <memory>

#include "nettypedef.hpp"
#include "utils/cthread.hpp"
#include "ccapsule.hpp"

namespace San2
{
	namespace Network
	{
        enum class InterfaceLineStatus : int
		{ 
			CONNECTED = 0, 
			DISCONNECTED = -1, // interface is ok, but line is not connected
			FAILURE = -2, // unrecoverable error
		};

		// It is up to the implementation to satisfy all the requirements.
		
		// Receiving is performed by passing InputQueue reference to derived base contructor
		// InputQueue should be obtained from CNode::getInputQueue();
		
		class CNetInterface
		{
		public:
			// NON-BLOCKING
			// starts the interface			
			virtual void up()=0; 
			
			// NON-BLOCKING
            // THREAD-SAFE
            // return value (true - queue was not full, false - queue full, drop occured)
			// NOTE: should likely put the capsule to its output queue, if queue is full then drop
			virtual bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr)=0;
			
			// NON-BLOCKING
            // THREAD-SAFE
			// returns pointer to the underlying thread, NULL if not availible
			virtual San2::Utils::CThread* getThread()=0; // might be replaced by other function in future releases
			
			// NON-BLOCKING
            // THREAD-SAFE
			// returns DefaultSanAddress if not availible
			virtual SanAddress getPeerAddress()=0;
			
			// NON-BLOCKING
            // THREAD-SAFE
			virtual SanAddress getInterfaceAddress()=0;			

            virtual InterfaceLineStatus getRXstate()=0;
            virtual InterfaceLineStatus getTXstate()=0;
		};
	}
}
