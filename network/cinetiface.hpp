#pragma once

#include "nettypedef.hpp"

namespace San2
{
	namespace Network
	{
		class CINetIface
		{
		public:
			virtual int sendCaplsule(const CCapsule &capsule);
		protected:
		
		private:
			SanAddress ifaceAddress;
		
		};
	}
}
