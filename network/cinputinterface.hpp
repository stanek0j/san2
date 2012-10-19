#pragma once

#include <memory>
#include "nettypedef.hpp"
#include "utils/cthread.hpp"

namespace San2
{
	namespace Network
	{
		class CInputInterface
		{
		public:
			// up() = interface start, MUST BE NON-BLOCKING
			virtual void up()=0; 
			
			virtual San2::Utils::CThread* getReceiverThread()=0;
			virtual SanAddress getInterfaceAddress()=0;			
		};
	}
}
