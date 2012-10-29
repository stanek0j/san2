
#pragma once

#include <set>
#include <memory>
#include <mutex>
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
			CNode(unsigned int inputQueueMaxSize, std::string nodeName);
		
			void run(); // start receiving capsules
			int addInterface(std::shared_ptr<San2::Network::CNetInterface> iface);
			
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule);
			
			template <class Rep, class Period>
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur);
			
			template <class Rep, class Period>
			bool tryInjectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, std::chrono::duration<Rep, Period> dur);
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& getInputQueue();
			
			std::string getNodeName() const;
		protected:
		 
		private:
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_inputQueue;
			std::set<std::shared_ptr<San2::Network::CNetInterface> > m_interfaces;
			
			std::chrono::milliseconds m_duration;
			
			std::mutex m_mutexInterfaces;
			const std::string m_nodeName; // used for orientation in terminal, nothing else
		};
	}

}
