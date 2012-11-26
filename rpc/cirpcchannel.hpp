
#pragma once

#include "utils/cvector.hpp"

namespace San2 
{
	namespace Rpc
	{
		class CIRpcChannel
		{
		public:
			// destructor must be virtual!
			virtual ~CIRpcChannel(){ /* intentionally empty */ };
		
			// TRUE = success
			virtual bool sendDatagram(const San2::Utils::bytes &data)=0;
			virtual bool recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec)=0;
		
		private:
			friend class CRpcExecutor;
            friend class CRpcInvoker;
			
			bool sendData(unsigned int funcId, const San2::Utils::bytes &data);
			bool recvData(unsigned int &funcIdOut, San2::Utils::bytes &out, unsigned int timeoutMsec);

		};
	}
}
