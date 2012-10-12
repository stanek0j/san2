#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <functional>
#include <string>

#include "citcpbuffer.hpp"
#include "utils/cthread.hpp"
#include "citcpreceiver.hpp"
#include "network/sockdefs.h"
#include "tcphelper.hpp"

namespace San2 
{
	namespace Tcp
	{
		class CITcpReciver;
		
		class CTcpClient : public CITcpBuffer
		{
			public:
				CTcpClient(const char *ip, const char *port, std::function<CITcpReceiver* (void)> proc, unsigned int timCON, unsigned int timRX, unsigned int timTX);
				~CTcpClient();
				TcpErrorCode open();
				TcpErrorCode send(char *data, int len);
				TcpErrorCode getErrorCode();
				
			protected:
				void run();
				TcpErrorCode runProc();
				
			private:
				TcpErrorCode read(char *data, unsigned int dataSize, unsigned int *bytesRead);
				
				const char *m_ip;
				const char *m_port;
				
				CITcpReceiver *absReceiver;
				unsigned int mTimCON, mTimRX, mTimTX;
				TcpErrorCode errcode;
				SNET_SOCKTYPE m_sock;
				

			// another msvc fix
			#ifdef LINUX
				CTcpClient(const CTcpClient& copyFromMe)=delete;
				CTcpClient& operator=(const CTcpClient& copyFromMe)=delete;
			#endif
		};
	}
}
