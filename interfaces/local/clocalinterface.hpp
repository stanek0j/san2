
#pragma once

namespace San2
{
	namespace Interfaces
	{
		public class CLocalInterface
		{
		public:
			// non blocking
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule);
			
			void up();
			bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr);
			San2::Utils::CThread* getThread();
			San2::Network::SanAddress getPeerAddress();
			San2::Network::SanAddress getInterfaceAddress();
		protected:
					
		private:
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& m_inputQueue;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_outputQueue;
			std::chrono::duration<int> m_duration;
			
			CTcpInterface& self(){return *this;}
		}
	}

}
