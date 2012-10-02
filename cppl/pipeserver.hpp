
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
#include "threadmanager.hpp"
#include "helper.hpp"
#include "abstractserverreceiver.hpp"

namespace CPPL 
{	
	class PipeServer : public CpThread
	{
		public:
			PipeServer(const char *pipeName, std::function<CPPL::AbstractServerReceiver* (void)> createAbstractServerReceiverProc, unsigned int timCON, unsigned int timRX, unsigned int timTX);
			ErrorCode getErrorCode();
			
		protected:
			void run();
            ErrorCode runProc();
			
			
		private:
			ThreadManager manager;
			const char *pipename;
			std::function<CPPL::AbstractServerReceiver* (void)> mCreateAbstractServerReceiverProc;
			unsigned int mTimCON, mTimRX, mTimTX;
			ErrorCode errcode;

		// another msvc fix
		#ifdef LINUX
			PipeServer(const PipeServer& copyFromMe)=delete;
			PipeServer& operator=(const PipeServer& copyFromMe)=delete;
		#endif
	};
}
