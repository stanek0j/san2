
#pragma once

#include <set>
#include <memory>
#include <mutex>
#include <vector>
#include "utils/cproducerconsumer.hpp"
#include "utils/cthread.hpp"
#include "network/cnetinterface.hpp"
#include "network/ccapsule.hpp"
#include "exec/cworkerpool.hpp"

namespace San2
{
	namespace Node
	{
		class CNode : public San2::Utils::CThread
		{
		public:
			CNode(unsigned int inputQueueMaxSize, unsigned int workQueueMaxSize, std::string nodeName, unsigned int timePOP);
		
			void run(); // start receiving capsules
			int addInterface(std::shared_ptr<San2::Network::CNetInterface> iface);
			
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule);
			bool injectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, unsigned int timeoutMsec);
			bool tryInjectCapsule(std::shared_ptr<San2::Network::CCapsule> capsule, San2::Utils::CThread *thr, unsigned int timeoutMsec);
			
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& getInputQueue();
			
			std::string getNodeName() const;

            
            //   it returns by-VALUE a copy of the set which does contain shared_pointers
            //   so the CNode cannot later affect return value(the set and pointers) be removing/adding interfaces
            //   >> changes to CNode interface list does not affect return value
            //   >> changes to interfaces themselves are shared between copy and CNode
            std::set<std::shared_ptr<San2::Network::CNetInterface> > getInterafces();
		protected:
		 
		private:
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_inputQueue;
			std::set<std::shared_ptr<San2::Network::CNetInterface> > m_interfaces;

			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_workQueue;
			San2::Exec::CWorkerPool *workerPool;
			
			std::mutex m_mutexInterfaces;
			const std::string m_nodeName; // used for orientation in terminal, nothing else
            unsigned int m_timePOP;
		};
	}

}
