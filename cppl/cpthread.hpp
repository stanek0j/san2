

#pragma once

#include <thread>
#include <atomic>

namespace CPPL
{
	class CpThread // iface
	{
	 public:
		CpThread();
		virtual ~CpThread();
		
		bool isTerminated(); // get flag state
		
		// TODO: unecessary and dangerous remove, use joinIfFinished() instead
		// bool isFinished(); // get flag state
		
		// actions
		void start(); // exec only once!!!
		void terminate(); // sets flag
		bool joinIfFinished();
		void terminateAndJoin();
		void join(); // blocks
	 
	 protected:	
		virtual void run()=0;
		
	 private:
		std::thread *thr;
		std::atomic_bool mTerminated; // tells this thread to terminate
		std::atomic_bool mFinished; // determines if thread is really finished
		
		void threadStartProc();
		
		// another msvc fix
		#ifdef LINUX
			CpThread(const CpThread& copyFromMe)=delete;
			CpThread& operator=(const CpThread& copyFromMe)=delete;
		#endif
	};

}
