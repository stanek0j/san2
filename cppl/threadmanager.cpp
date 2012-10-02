
#include <mutex>

#include "threadmanager.hpp"
#include "cpthread.hpp"
#include "pipechannel.hpp"

namespace CPPL
{

	// TODO: Very ugly needs nicer code
#ifdef LINUX
	void ThreadManager::startThread(int sockt, std::function<CPPL::AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timRX, unsigned int timTX)
	{
		mMutex.lock();
			// anything inside mutex must not throw, otherwise deadlock
			PipeChannel *chann = new PipeChannel(sockt, createAbstractServerReceiverProc, timRX, timTX);
			mThreads.insert(chann);
			chann->start();
		mMutex.unlock();
	}
#endif

#ifdef WINDOWS
	void ThreadManager::startThread(HANDLE hPipe, std::function<CPPL::AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timRX, unsigned int timTX)
	{
		mMutex.lock();
			// anything inside mutex must not throw, otherwise deadlock
			PipeChannel *chann = new PipeChannel(hPipe, createAbstractServerReceiverProc, timRX, timTX);
			mThreads.insert(chann);
			chann->start();
		mMutex.unlock();
	}
#endif
	
	int ThreadManager::joinIfFinished()
	{
		int count = 0;
		
		mMutex.lock();
			// anything inside mutex must not throw, otherwise deadlock
			// std::set<PipeChannel*>::iterator iter;
			
			/*
			for(iter = mThreads.begin(); iter != mThreads.end(); iter++)
			{
				if ((*iter)->joinIfFinished())
				{ 
					count++;
					delete (*iter);
					mThreads.erase(iter); // neposune se to sptane????
				}
			}
			*/

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

			/*
			for (iter = mThreads.begin(); iter != mThreads.end() ; )
			{
				if ((*iter)->joinIfFinished())
				{
					delete *iter;
					mThreads.erase(iter++);
				}
				else
				{
					++iter;
				}
			}
			*/

		mMutex.unlock();
		
		return count;
	}
	
	void ThreadManager::terminateAndJoin()
	{
		mMutex.lock();
			// anything inside mutex must not throw, otherwise deadlock
			/*
			std::set<PipeChannel*>::iterator iter;
			for(iter = mThreads.begin(); iter != mThreads.end(); iter++)
			{
				(*iter)->terminateAndJoin();
				delete(*iter);
			}
			mThreads.erase(mThreads.begin(), mThreads.end());
			*/
			for (auto it = mThreads.begin(); it != mThreads.end();)
			{
				(*it)->terminateAndJoin();
				delete *it;
				mThreads.erase(it++);
			}

		mMutex.unlock();
	}

	int ThreadManager::threadCount()
	{
		long cnt;
		
		mMutex.lock();
			// anything inside mutex must not throw, otherwise deadlock
			cnt = mThreads.size();
		mMutex.unlock();
		
		return cnt;
	}
}
