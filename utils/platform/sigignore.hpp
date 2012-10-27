
#pragma once

#ifdef LINUX

namespace San2
{
	namespace Utils
	{
		int san_signal_ignore(int signum);
		int san_ignore_sigpipe();
	}
}

#endif
