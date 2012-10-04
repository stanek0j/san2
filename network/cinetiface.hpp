#pragma once

#include "nettypedef.hpp"

namespace San2
{
	namespace Network
	{
		class CINetIface
		{
		public:
			virtual int sendCapsule(CCapsule &capsule)=0; // mozna to zmenim na pointer nebo auto_ptr
			virtual SanAddress getInterfaceAddress()=0;	
	
		};
	}
}
