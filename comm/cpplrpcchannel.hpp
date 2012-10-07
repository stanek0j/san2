
#pragma once

#include "rpc/cirpcchannel.hpp"
#include "stream/clenvalueseparator.hpp"
#include "cpplstreamrw.hpp"

namespace San2
{
	namespace Comm
	{
		class CpplRpcChannel : public San2::Rpc::CIRpcChannel
		{
		public:
			CpplRpcChannel(San2::Cppl::BufferProcessor *bp);
			// destructor must be virtual!
			
			virtual ~CpplRpcChannel()
			{ 
				delete m_separator;
			}

			// TRUE = success
			virtual bool sendDatagram(const San2::Utils::bytes &data);
			virtual bool recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec);

		private:
			San2::Cppl::BufferProcessor *m_bp;
			CpplStreamRW m_rw;
			San2::Stream::CLenValueSeparator *m_separator;
		};
	}
}
