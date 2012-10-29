
#pragma once

#include "utils/cthread.hpp"
#include "utils/cproducerconsumer.hpp"

#include <chrono>
#include <ratio>

namespace San2
{
	namespace Test
	{
		template <class T>
		class CConsumer : public San2::Utils::CThread
		{
			public:
				CConsumer(San2::Utils::CProducerConsumer<T> &pc)
					: m_pc(pc)
				{
					
				}
				
				void run()
				{
					std::chrono::milliseconds dur(2000);
					//printf("consumer started\n");
					T t;
					while (!isTerminated()) m_pc.pop(&t, this, dur);
				}
			protected:
				
			private:
				San2::Utils::CProducerConsumer<T> &m_pc;
		};
	
	}
}
