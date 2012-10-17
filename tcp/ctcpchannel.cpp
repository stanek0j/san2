
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/un.h>
#endif

#ifdef WIN32
    #include <WinSock2.h>
	#include <windows.h>
#endif


#include <string>

#include "ctcpchannel.hpp"
#include "tcphelper.hpp"
#include "network/sockdefs.h"

#define CPPL_PIPECHANN_RXBUFSIZE 1500

namespace San2 { namespace Tcp {
	
		CTcpChannel::~CTcpChannel()
		{
			// empty
		}
		
		TcpErrorCode CTcpChannel::getErrorCode()
		{
			return errcode;
		}
	
		CTcpChannel::CTcpChannel(SNET_SOCKTYPE sock, unsigned long addr, unsigned int timRX, unsigned int timTX) :
			m_sock(sock),
            m_addr(addr),
			m_timRX(timRX),
			m_timTX(timTX),
            errcode(TcpErrorCode::SUCCESS)
		{
			
		}
		
		void CTcpChannel::run()
		{
			errcode = receive();    
			SNET_SOCKCLOSE(m_sock);
			m_sock = SNET_BADSOCKET;
		}

		TcpErrorCode CTcpChannel::send(char *data, int len)
		{
			return sock_sendall_stream(m_sock, data, &len, m_timTX, this);
		}
		
		TcpErrorCode CTcpChannel::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
        {
            return sock_read_stream(m_sock, this, data, dataSize, bytesRead, m_timRX);
        }
}} // ns
