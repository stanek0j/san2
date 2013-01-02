
#pragma once
#include <mutex>

namespace San2
{
	namespace Utils
	{
		template <class T>
		class Atomic
		{
			public:
				Atomic()
				{
					
				}
			
				Atomic(T value) :
					m_variable(value)
				{
					
				}
			
				T operator=(T value)
				{
					std::lock_guard<std::mutex> lock(m_mutexVariable);
					m_variable = value;
					return m_variable;
				}
				
				operator T()
				{
					std::lock_guard<std::mutex> lock(m_mutexVariable);
					return m_variable;	
				}	
				
			private:
				std::mutex m_mutexVariable;
				T m_variable;
			
			#ifndef WINDOWS
				Atomic& operator=(const Atomic&)=delete;
				Atomic& operator=(const Atomic&)volatile=delete;
			#endif
		};
	}	
	
}
