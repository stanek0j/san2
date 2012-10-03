
#pragma once

#include <string>
#include <atomic>
#include <set>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include <functional>
#include "pipechannel.hpp"
#include "utils/cthread.hpp"
#include "utils/cthreadmanager.hpp"
#include "helper.hpp"
#include "abstractserverreceiver.hpp"

namespace San2 
{	
	namespace Cppl
	{
		class PipeServer : public San2::Utils::CThread
		{
			public:
				PipeServer(const char *pipeName, std::function<AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timCON, unsigned int timRX, unsigned int timTX);
				ErrorCode getErrorCode();
				
			protected:
				void run();
				ErrorCode runProc();
				
				
			private:
				San2::Utils::CThreadManager manager;
				const char *pipename;
				std::function<AbstractServerReceiver* (void)> mCreateAbstractServerReceiverProc;
				unsigned int mTimCON, mTimRX, mTimTX;
				ErrorCode errcode;

			// another msvc fix
			#ifdef LINUX
				PipeServer(const PipeServer& copyFromMe)=delete;
				PipeServer& operator=(const PipeServer& copyFromMe)=delete;
			#endif
		};
	}
}
