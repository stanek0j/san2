
#pragma once

#include "rpc/cirpcchannel.hpp"
#include "tcp/citcpbuffer.hpp"
#include "stream/clenvalueseparator.hpp"
#include "tcpstreamrw.hpp"

namespace San2
{
	namespace Comm
	{
		class TcpRpcChannel : public San2::Rpc::CIRpcChannel
		{
		public:
			TcpRpcChannel(San2::Tcp::CITcpBuffer *bp);
			// destructor must be virtual!
			
			virtual ~TcpRpcChannel()
			{ 
				delete m_separator;
			}

			// TRUE = success
			virtual bool sendDatagram(const San2::Utils::bytes &data);
			virtual bool recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec);

		private:
			San2::Tcp::CITcpBuffer *m_bp;
			TcpStreamRW m_rw;
			San2::Stream::CLenValueSeparator *m_separator;
		};
	}
}
