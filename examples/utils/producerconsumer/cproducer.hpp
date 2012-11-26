
#pragma once

#include "utils/cthread.hpp"
#include "utils/cproducerconsumer.hpp"

namespace San2
{
	namespace Test
	{
		template <class T>
		class CProducer : public San2::Utils::CThread
		{
			public:
				CProducer(San2::Utils::CProducerConsumer<T> &pc)
					: m_pc(pc)
				{
					
				}
				
				void run()
				{
					//printf("producer started\n");
					T t;
					while (!isTerminated()) 
					{
						m_pc.push(t, this, 2000);
					}
				}
			
			protected:
			
			private:
				San2::Utils::CProducerConsumer<T> &m_pc;
		};
	
	}
}
