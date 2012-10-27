
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "sigignore.hpp"
#include "utils/log.h"

#ifdef LINUX

namespace San2 { namespace Utils {

void ignore_handler(int signum, siginfo_t *info, void *ptr)
{
	// ignore everything
	return; 
}

int san_signal_ignore(int signum)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = ignore_handler;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(signum, &act, NULL))
	{ 
		FILE_LOG(logERROR) << "san_signal_ignore():sigaction() FAILED";
		return -1;
	}
	return 0;
}

int san_ignore_sigpipe()	
{
	return san_signal_ignore(SIGPIPE);
}

}} // ns

#endif
