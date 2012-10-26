#pragma once

#include <memory>
#include "nettypedef.hpp"
#include "utils/cthread.hpp"
#include "ccapsule.hpp"

namespace San2
{
	namespace Network
	{
		class CNetInterface
		{
		public:
			// up() = MUST be implemented as NON-BLOCKING
			virtual void up()=0; 
			// sendCapsule() MUST be implemented as NON-BLOCKING
			virtual bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr)=0;
			virtual San2::Utils::CThread* getThread()=0;
			virtual SanAddress getPeerAddress()=0;
			virtual SanAddress getInterfaceAddress()=0;			
		};
	}
}
