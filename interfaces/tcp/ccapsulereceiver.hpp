
#pragma once

#include <memory>
#include <mutex>

#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "tcp/csingletcpserver.hpp"
#include "tcp/tcphelper.hpp"
#include "network/nettypedef.hpp"
#include "utils/cproducerconsumer.hpp"
#include "network/ccapsule.hpp"

namespace San2
{
	namespace Interfaces
	{
		class CTcpInterface;
		
		class CCapsuleReceiver : public San2::Tcp::CSingleTcpServer
		{
		  public:
			CCapsuleReceiver(CTcpInterface &iface, const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue);
			virtual ~CCapsuleReceiver();
		  protected:
			
			San2::Tcp::TcpErrorCode receive();
		  private:
			bool parseFirstMessage(const San2::Utils::bytes &data);

			 // another msvc fix
			#ifdef LINUX
				CCapsuleReceiver(const CCapsuleReceiver& copyFromMe)=delete;
				CCapsuleReceiver& operator=(const CCapsuleReceiver& copyFromMe)=delete;
			#endif
			
			CTcpInterface &m_iface;
			San2::Comm::StreamRpcChannel *m_rpcChannel;
			San2::Rpc::CRpcExecutor *m_rpcexec;
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& m_inputQueue;
		};
	}
}
