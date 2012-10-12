#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <functional>
#include <string>

#include "utils/cthread.hpp"
#include "network/sockdefs.h"
#include "tcphelper.hpp"
#include "citcpbuffer.hpp"

namespace San2 
{
	namespace Tcp
	{	
		class CTcpClient : public CITcpBuffer
		{
			public:
				CTcpClient(const char *ip, const char *port, unsigned int timCON, unsigned int timRX, unsigned int timTX);
				~CTcpClient();
				TcpErrorCode open();
				TcpErrorCode send(char *data, int len);
				TcpErrorCode getErrorCode();
				
			protected:
				void run();
				virtual TcpErrorCode receive()=0;
				TcpErrorCode runProc();
				
				TcpErrorCode read(char *data, unsigned int dataSize, unsigned int *bytesRead);
			private:
				
				
				const char *m_ip;
				const char *m_port;
				
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
