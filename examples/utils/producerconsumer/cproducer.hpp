
#pragma once

#include <chrono>
#include <ratio>

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
					std::chrono::milliseconds dur(2000);
					//printf("producer started\n");
					T t;
					while (!isTerminated()) 
					{
						m_pc.push(t, this, dur);
					}
				}
			
			protected:
			
			private:
				San2::Utils::CProducerConsumer<T> &m_pc;
		};
	
	}
}
