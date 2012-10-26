
#pragma once

namespace San2
{
	namespace Interfaces
	{
		public class CLocalInterface
		{
		public:
			San2::Utils::CThread* getThread();
			
			// No idea how to fix this mess 
			void up(); // CTcpInterface
			
			
			
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule);
			bool sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr);
			
			
			San2::Network::SanAddress getPeerAddress();
			San2::Network::SanAddress getInterfaceAddress();
		protected:
					
		private:
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& m_inputQueue;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_outputQueue;
			std::chrono::duration<int> m_duration;
			
			std::mutex m_mutexPeerAddress;
			
			CTcpInterface& self(){return *this;}
			
			friend class CCapsuleReceiver;
			void setPeerAddress(const San2::Network::SanAddress &address); // mutexed
			
		}
	}

}
