
#pragma once

#include <functional>
#include "utils/cthread.hpp"
#include "utils/cthreadmanager.hpp"
#include "network/sockdefs.h"
#include "network/nettypedef.hpp"
#include "tcphelper.hpp"
#include "citcpreceiver.hpp"

namespace San2
{
	namespace Tcp
	{
		class CTcpServer : public San2::Utils::CThread
		{
			public:
				CTcpServer(const char *ip, const char *port, std::function<CITcpReceiver* (void)> proc, unsigned int timCON, unsigned int timRX, unsigned int timTX);
				
			protected:
				void run();
				TcpErrorCode runProc();
				TcpErrorCode getErrorCode();
			private:
				San2::Utils::CThreadManager manager;
				
				const char *m_ip;
				const char *m_port;
				
				std::function<CITcpReceiver* (void)> m_proc;
				unsigned int mTimCON, mTimRX, mTimTX;
				TcpErrorCode errcode;

			// another msvc fix
			#ifdef LINUX
				CTcpServer(const CTcpServer& copyFromMe)=delete;
				CTcpServer& operator=(const CTcpServer& copyFromMe)=delete;
			#endif
		};
	}
}
