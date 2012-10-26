
#pragma once

// This multiple producer - multi consumer

#include <queue>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <ratio>

#include <iostream>
#include <stdio.h>

#include "cthread.hpp"

namespace San2
{
	namespace Utils
	{
		template < class T, class Container = std::deque<T> >
		class CProducerConsumer
		{
			private:
				unsigned long m_maxQueueSize;
				std::queue<T, Container> m_que;
				std::mutex m_mutex; 
				std::mutex m_rwmut;  // This is necessary because of MULTIPLE producers and consumers
				std::condition_variable m_condNotEmpty;
				std::condition_variable m_condNotFull;
				
				inline bool isEmpty() // must be called in mutex
				{
					unsigned int cnt = m_que.size();
					return (cnt == 0);
				}
				
				inline bool isFull() // must be called in mutex
				{
					unsigned int cnt = m_que.size();
					return (cnt == m_maxQueueSize);
				}
				
			public:
				CProducerConsumer(unsigned long maxQueueSize) :
					m_maxQueueSize(maxQueueSize)
				{
					
				}

				~CProducerConsumer()
				{
					
				}

				template <class Rep, class Period>
				int try_push(const T &item, CThread *thr, std::chrono::duration<Rep, Period> duration)
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					
					while(isFull())
					{
						if (m_condNotEmpty.wait_for(lock, duration) == std::cv_status::timeout)	return -2;
						if (thr->isTerminated()) return -1;
					}
					
					if (thr->isTerminated()) return -1;
					
					m_rwmut.lock();
						m_que.push(item);
					m_rwmut.unlock();	
					
					m_condNotFull.notify_all();
					return 0;
				}

				template <class Rep, class Period>
				int push(const T &item, CThread *thr, std::chrono::duration<Rep, Period> duration)
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					while(isFull())
					{
						m_condNotEmpty.wait_for(lock, duration);
						if (thr->isTerminated()) return -1;
					}
					if (thr->isTerminated()) return -1;
					
					m_rwmut.lock();
						m_que.push(item);
					m_rwmut.unlock();
					
					m_condNotFull.notify_all();
					return 0;
				}
				
				int push(const T &item)
				{
					std::unique_lock<std::mutex> lock(m_mutex);
					while(isFull())	m_condNotEmpty.wait(lock);
					
					m_rwmut.lock();
						m_que.push(item);
					m_rwmut.unlock();
					
					m_condNotFull.notify_all();
					return 0;
				}

				template <class Rep, class Period>
				int try_pop(T *out, CThread *thr, std::chrono::duration<Rep, Period> duration)
				{
					std::unique_lock<std::mutex> lock(m_mutex);				
				    while (isEmpty())
				    { 
						if (m_condNotFull.wait_for(lock, duration) == std::cv_status::timeout) return -2;
						if (thr->isTerminated()) return -1; 
					}
					if (thr->isTerminated()) return -1; 
										
					m_rwmut.lock();
						*out = m_que.front();
						m_que.pop();
					m_rwmut.unlock();
					
					m_condNotEmpty.notify_all();					
					return 0;
				}

				template <class Rep, class Period>
				int pop(T *out, CThread *thr, std::chrono::duration<Rep, Period> duration)
				{
					std::unique_lock<std::mutex> lock(m_mutex);				
				    while (isEmpty())
				    { 
						m_condNotFull.wait_for(lock, duration);
						if (thr->isTerminated()) return -1;
					}
					
					if (thr->isTerminated()) return -1;
					
					m_rwmut.lock();
						*out = m_que.front();
						m_que.pop();
					m_rwmut.unlock();
					
					m_condNotEmpty.notify_all();					
					return 0;
				}
				
				
				int pop(T *out)
				{
					std::unique_lock<std::mutex> lock(m_mutex);				
				    while (isEmpty()) m_condNotFull.wait(lock);
					
					m_rwmut.lock();
						*out = m_que.front();
						m_que.pop();
					m_rwmut.unlock();
					
					m_condNotEmpty.notify_all();					
					return 0;
				}

				// informative only
				unsigned long size()
				{
					std::lock_guard<std::mutex> lock(m_mutex);
					return m_que.size();
				}
		};
	}
}
