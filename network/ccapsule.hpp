
#pragma once

#include "nettypedef.hpp"

namespace San2
{
	namespace Network
	{
		class CCapsule
		{
		public:
			void setSourceAddress(const SanAddress &srcAddress);
			void setDestinationAddress(const SanAddress &dstAddress);
			void setHopLimit(const SanHops &maxHops);
			
			SanAddress getSourceAddress();
			SanAddress getDestinationAddress();
			SanHops getHopLimit();
			
			void setDX(bool dx); // Destination eXecute
			void setEX(bool ex); // Everywhere eXecute
			
			San2::Utils::bytes data; // set directly
		protected:
		
		private:
			SanAddress m_srcAddress;
			SanAddress m_dstAddress;
			SanApplicationId m_appId;
			SanFlags m_flags;
			SanHops m_hopLimit;
		};
	
		
	}
}
