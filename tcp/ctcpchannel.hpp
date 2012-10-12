
#pragma once

#include "utils/cthread.hpp"
#include "utils/cthreadmanager.hpp"
#include "tcphelper.hpp"
#include "network/sockdefs.h"
#include "network/nettypedef.hpp"
#include "citcpreceiver.hpp"

namespace San2
{
	namespace Tcp
	{
		class CTcpChannel : public CITcpBuffer
		{
		public:
			CTcpChannel(SNET_SOCKTYPE sock, std::function<CITcpReceiver* (void)> createAbstractReceiverProc, unsigned long s_addr, unsigned int timRX, unsigned int timTX);
			virtual ~CTcpChannel();
		protected:
			void run();
			TcpErrorCode send(char *data, int len);
			TcpErrorCode getErrorCode();
			
		private:
			TcpErrorCode read(char *data, unsigned int dataSize, unsigned int *bytesRead);

			SNET_SOCKTYPE m_sock;
			CITcpReceiver* absReceiver;
			unsigned int m_timRX, m_timTX;
			TcpErrorCode errcode;
			
			// another msvc fix
			#ifdef LINUX
				CTcpChannel(const CTcpChannel& copyFromMe)=delete;
				CTcpChannel& operator=(const CTcpChannel& copyFromMe)=delete;
			#endif
			
		};
	}
}
