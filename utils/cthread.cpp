
#include "cthread.hpp"
#include <thread>

#ifdef LINUX
	San2::Utils::CThread::CThread() :
		thr(NULL),
		mTerminated(false),
		mFinished(false)
	{
	
	}
#endif

// MSVC bug workaround
#ifdef WINDOWS
	San2::Utils::CThread::CThread() :
		thr(NULL)
	{
		// another MSVC bug
		mTerminated = false;
		mFinished = false;
	}
#endif

San2::Utils::CThread::~CThread()
{
	if (thr != NULL) delete thr;
}

void San2::Utils::CThread::start()
{
	// TODO: check if thread is not already running
	mTerminated = false;
	mFinished = false;
	thr = new std::thread(&CThread::threadStartProc, this);
}

bool San2::Utils::CThread::joinIfFinished()
{
	// TODO check if there is thread already running
	if (mFinished)
	{
		join(); // calls this.join() not std::thread::join() !!!
		return true;
	}
	else return false;
}

void San2::Utils::CThread::join()
{
	thr->join();
}

void San2::Utils::CThread::terminateAndJoin()
{
	terminate();
	join(); // calls this.join() not std::thread::join() !!!
}

void San2::Utils::CThread::terminate()
{
	mTerminated = true;
}

bool San2::Utils::CThread::isTerminated()
{
	return mTerminated;
}

/*
bool CThread::isFinished()
{
	return mFinished;
}
*/

// This procedure runs in the new thread
void San2::Utils::CThread::threadStartProc()
{
	run();
	mFinished = true;
}

