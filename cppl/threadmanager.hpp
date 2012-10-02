
#pragma once

#include <thread>
#include <atomic>
#include <set>
#include <utility>
#include <mutex> 
#include <functional>

#include "cpthread.hpp"
#include "pipechannel.hpp"

namespace CPPL
{	
	class ThreadManager
	{
		public:
		
		#ifdef LINUX
			void startThread(int sockt, std::function<CPPL::AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timRX, unsigned int timTX);
		#endif

		#ifdef WINDOWS
			void startThread(HANDLE hPipe, std::function<CPPL::AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timRX, unsigned int timTX);
		#endif

			// Operations on set
			int joinIfFinished();
			void terminateAndJoin();
			
			int threadCount();
		private:
			std::set<PipeChannel*> mThreads;
			std::mutex mMutex;
			
	};
	
	
}
