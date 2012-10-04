
#pragma once

#include <thread>
#include <atomic>
#include <set>
#include <utility>
#include <mutex> 
#include <functional>

#include "cthread.hpp"

namespace San2
{	
	namespace Utils
	{
		class CThreadManager
		{
			public:
			
			template <class CThreadDeriverType>
			CThreadDeriverType *startThread(std::function<CThreadDeriverType* (void)> createThreadFunction)
			{
				std::lock_guard<std::mutex> lock(mMutex);
				
				CThreadDeriverType *thr = createThreadFunction();
				mThreads.insert(thr);
				thr->start();
				return thr;
			}

			// Operations on set
			int joinIfFinished()
			{
				int count = 0;
				std::lock_guard<std::mutex> lock(mMutex);
				// http://stackoverflow.com/questions/8997503/map-set-iterator-not-incrementablemap-set-iterator-not-incrementable
				for (auto it = mThreads.begin(); it != mThreads.end();)
				{
					if ((*it)->joinIfFinished())
					{
						count++;
						delete *it;
						mThreads.erase(it++);
					}
					else
					{
						++it;
					}
				}
				return count;
			}
			
			
			void terminateAndJoin()
			{
				std::lock_guard<std::mutex> lock(mMutex);
				for (auto it = mThreads.begin(); it != mThreads.end();)
				{
					(*it)->terminateAndJoin();
					delete *it;
					mThreads.erase(it++);
				}
			}
			
			int threadCount()
			{
				std::lock_guard<std::mutex> lock(mMutex);
				return mThreads.size();
			}
		
			private:
				std::set<CThread*> mThreads;
				std::mutex mMutex;
			
		};
	}
}
