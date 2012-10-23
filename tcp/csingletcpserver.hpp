
#pragma once

#include <functional>
#include "utils/cthread.hpp"
#include "tcphelper.hpp"
#include "citcpbuffer.hpp"
#include "network/sockdefs.h"
#include "network/nettypedef.hpp"


// This will be used to implement node interface, the receiver part
// it is important to make this fault-tolerant as the remote node
// could temporarily disconnect and then reconnect

namespace San2
{
	namespace Tcp
	{	
		class CSingleTcpServer : public San2::Utils::CThread, public CITcpBuffer
		{
			public:
				CSingleTcpServer(const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX);
				
			protected:
				void run();
				TcpErrorCode runProc();
				TcpErrorCode getErrorCode();
				virtual TcpErrorCode receive()=0;
				struct sockaddr_in getRemoteAddress();
				
			private:
				
				std::string m_ip;
				std::string m_port;
				struct sockaddr_in m_remoteAddress;
				
				unsigned int mTimCON, mTimRX, mTimTX;
				TcpErrorCode errcode;
				SNET_SOCKTYPE listenSocket, acceptSocket;

			// another msvc fix
			#ifdef LINUX
				CSingleTcpServer(const CSingleTcpServer& copyFromMe)=delete;
				CSingleTcpServer& operator=(const CSingleTcpServer& copyFromMe)=delete;
			#endif
		};
	}
}
