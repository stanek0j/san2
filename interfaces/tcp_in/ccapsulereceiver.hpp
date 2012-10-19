
#pragma once

#include <memory>

#include "comm/tcprpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "tcp/ctcpchannel.hpp"
#include "tcp/tcphelper.hpp"
#include "utils/cproducerconsumer.hpp"
#include "network/ccapsule.hpp"

namespace San2
{
	namespace Interfaces
	{
		class CCapsuleReceiver : public San2::Tcp::CTcpChannel
		{
		  public:
			CCapsuleReceiver(SNET_SOCKTYPE handle, unsigned long addr, unsigned int timRX, unsigned int timTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue);
			virtual ~CCapsuleReceiver();
		  protected:
			
			San2::Tcp::TcpErrorCode receive();
		  private:

			 // another msvc fix
			#ifdef LINUX
				CCapsuleReceiver(const CCapsuleReceiver& copyFromMe)=delete;
				CCapsuleReceiver& operator=(const CCapsuleReceiver& copyFromMe)=delete;
			#endif
			
			San2::Comm::TcpRpcChannel *m_rpcChannel;
			San2::Rpc::CRpcExecutor *m_rpcexec;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& m_inputQueue;
		};
	}
}
