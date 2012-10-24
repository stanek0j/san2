
#pragma once

#include <set>
#include <memory>
#include "utils/cproducerconsumer.hpp"
#include "utils/cthread.hpp"
#include "network/cnetinterface.hpp"
#include "network/ccapsule.hpp"

namespace San2
{
	namespace Node
	{
		class CNode : public San2::Utils::CThread
		{
		public:
			CNode(unsigned int inputQueueMaxSize);
		
			void run(); // start receiving capsules
			int addInterface(std::shared_ptr<San2::Network::CNetInterface> iface);
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& getInputQueue();
		protected:
		 
		private:
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_inputQueue;
			std::set<std::shared_ptr<San2::Network::CNetInterface> > m_interfaces;
			
			std::chrono::duration<int> m_duration;
		};
	}

}
