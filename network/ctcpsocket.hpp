
#pragma once

#include "nettypedef.hpp"
#include "sockdefs.h"

#include <list>

namespace San2
{
	namespace Network
	{
		class CTcpSocket
		{
		public:
			CTcpSocket(const IPv4Address &toBind);
			static std::list<IPv4Address> getIPv4InterfaceList();
			
		protected:
		
		private:
			SNET_SOCKTYPE sock;
		};
	
		
	}
}
