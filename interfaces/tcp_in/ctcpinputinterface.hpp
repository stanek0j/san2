#pragma once

#include <string>
#include <memory>
#include "network/nettypedef.hpp"
#include "network/cinputinterface.hpp"
#include "network/ccapsule.hpp"
#include "utils/cproducerconsumer.hpp"
#include "tcp/ctcpserver.hpp"

namespace San2
{
	namespace Interfaces
	{
		class CTcpInputInterface : San2::Network::CInputInterface
		{
		public:
			CTcpInputInterface(const std::string &ip, const std::string &port, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue);
			San2::Utils::CThread* getReceiverThread();
			void up();
		protected:
					
		private:
			std::string m_ip;
			std::string m_port;
			unsigned int m_timeCON, m_timeRX, m_timeTX;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& m_inputQueue;
			San2::Tcp::CTcpServer tcpsrv;
		};
	}
}
