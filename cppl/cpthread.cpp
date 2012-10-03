
#include "cpthread.hpp"
#include <thread>

namespace San2 { namespace Cppl {
	
	#ifdef LINUX
		CpThread::CpThread() :
			thr(NULL),
			mTerminated(false),
			mFinished(false)
		{
		
		}
	#endif

	// MSVC bug workaround
	#ifdef WINDOWS
		CpThread::CpThread() :
			thr(NULL)
		{
			// another MSVC bug
			mTerminated = false;
			mFinished = false;
		}
	#endif
	
	CpThread::~CpThread()
	{
		if (thr != NULL) delete thr;
	}

	void CpThread::start()
	{
		// TODO: check if thread is not already running
		mTerminated = false;
		mFinished = false;
		thr = new std::thread(&CpThread::threadStartProc, this);
	}
	
	bool CpThread::joinIfFinished()
	{
		// TODO check if there is thread already running
		if (mFinished)
		{
			join(); // calls this.join() not std::thread::join() !!!
			return true;
		}
		else return false;
	}
	
	void CpThread::join()
	{
		thr->join();
	}
	
	void CpThread::terminateAndJoin()
	{
		terminate();
		join(); // calls this.join() not std::thread::join() !!!
	}
	
	void CpThread::terminate()
	{
		mTerminated = true;
	}
	
	bool CpThread::isTerminated()
	{
		return mTerminated;
	}
	
	/*
	bool CpThread::isFinished()
	{
		return mFinished;
	}
	*/
	
	// This procedure runs in the new thread
	void CpThread::threadStartProc()
	{
		run();
		mFinished = true;
	}
}} // ns
