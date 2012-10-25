
#pragma once

#include "rpc/cirpcchannel.hpp"
#include "stream/clenvalueseparator.hpp"
#include "stream/cistreamrw.hpp"

namespace San2
{
	namespace Comm
	{
		class StreamRpcChannel : public San2::Rpc::CIRpcChannel
		{
		public:
			StreamRpcChannel(San2::Stream::CIStreamRW &rw);
			// destructor must be virtual!
			virtual ~StreamRpcChannel(){};

			// TRUE = success
			virtual bool sendDatagram(const San2::Utils::bytes &data);
			virtual bool recvDatagram(San2::Utils::bytes &out, unsigned int timeoutMsec);
		private:
			//San2::Stream::CIStreamRW &m_rw;
			San2::Stream::CLenValueSeparator m_separator;
		};
	}
}
