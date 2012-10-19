
#pragma once

#include <memory>
#include "utils/cproducerconsumer.hpp"

namespace San2
{
	namespace Node
	{
		class CNode
		{
		public:
		
		protected:
		 
		private:
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > m_inputQueue;
			
		};
	}

}
