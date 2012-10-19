#pragma once

#include <memory>
#include "nettypedef.hpp"
#include "utils/cthread.hpp"

namespace San2
{
	namespace Network
	{
		class COutputInterface : public San2::Utils::CThread
		{
		public:
			 // MUST be implemented as non-blocking!!!!!
			 // (derived class will likely put it into its own output queue)
			 // Return value: true = success
			bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule)=0;
			virtual SanAddress getInterfaceAddress()=0;			
		};
	}
}
