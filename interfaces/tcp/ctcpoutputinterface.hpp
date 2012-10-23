#pragma once

#include <memory>
#include <string>
#include "network/nettypedef.hpp"
#include "network/coutputinterface.hpp"
#include "network/ccapsule.hpp"
#include "utils/cthread.hpp"
#include "utils/cproducerconsumer.hpp"
#include "tcp/ctcpclient.hpp"
#include "comm/tcprpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"

namespace San2
{
	namespace Interfaces
	{
		class CTcpOutputInterface : public San2::Network::COutputInterface, public San2::Tcp::CTcpClient
		{
		public:
			// timeRX is practically ignored, as no receive operation will be in progress
			CTcpOutputInterface(const std::string &ip, const std::string &port, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, unsigned long maxQueueSize);
			virtual ~CTcpOutputInterface();
			 // MUST be implemented as non-blocking!!!!!
			 // (derived class will likely put it into its own output queue)
			 // Return value: true = success
			 
			// No idea how to fix this mess 
			void up();
			void run();
			void received();
			
			bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr);
			virtual San2::Network::SanAddress getInterfaceAddress();			
			virtual San2::Utils::CThread* getSenderThread();		
		
		private:		
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_outputQueue;
			
			unsigned int m_timeCON, m_timeRX, m_timeTX;
			
			San2::Comm::TcpRpcChannel *m_rpcChannel;
			San2::Rpc::CRpcExecutor *m_rpcexec;
			std::chrono::duration<int> m_duration;
		};
	}
}
