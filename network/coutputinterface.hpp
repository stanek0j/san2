#pragma once

#include <memory>
#include "nettypedef.hpp"
#include "network/ccapsule.hpp"
#include "utils/cthread.hpp"

namespace San2
{
	namespace Network
	{
		class COutputInterface
		{
		public:
			 // MUST be implemented as non-blocking!!!!!
			 // (derived class will likely put it into its own output queue)
			 // Return value: true = success
			 
			virtual bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr)=0;
			virtual void up()=0; // starts interface, must be implemented as non-blocking
			
			
			virtual SanAddress getInterfaceAddress()=0;	
			virtual San2::Utils::CThread* getSenderThread()=0;		
		};
	}
}
