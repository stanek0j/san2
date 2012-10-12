
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
#endif

#include "tcphelper.hpp"

namespace San2 { namespace Tcp {
	
		// miliseconds to timeval struct
		void ms2tv(unsigned long msec, struct timeval *tv) 
		{
			tv->tv_sec = msec / 1000;
			tv->tv_usec = (msec % 1000) * 1000;
		}

		TcpErrorCode sock_sendall_stream(SNET_SOCKTYPE sock, const char *buf, int *len, unsigned int timeoutMsec, San2::Utils::CThread *thr)
		{
			if (sock == -1) return TcpErrorCode::FAILURE;
			fd_set fds;
			struct timeval tv;
			int selRval;
			
			int total = 0;        // how many bytes we've sent
			int bytesleft = *len; // how many we have left to send
			int n = 0; // =0 kvuli warningu

			while(total < *len) 
			{
				//printf("total: %d  len: %d\n", total, *len);
				FD_ZERO(&fds);
				FD_SET(sock, &fds);
				
				ms2tv(timeoutMsec, &tv);
				
				errno = 0;
				selRval = select(sock+1, NULL, &fds, NULL, &tv);
				
				if (errno == EINTR) 
				{
					if (thr->isTerminated()) return TcpErrorCode::TERMINATED;
					continue; // INTERRUPT continue
				}
				
				if (thr->isTerminated()) return TcpErrorCode::TERMINATED;
				if (selRval == 0) continue; // timeout, terminated check already performed
				if (selRval < 0) return TcpErrorCode::FAILURE;
				
				errno = 0;
				n = send(sock, buf+total, bytesleft, 0);
				if (errno == EPIPE || errno == ECONNRESET) return TcpErrorCode::PEER_DISCONNECT;
				if (n == -1) break;
				total += n;
				bytesleft -= n;
			}

			*len = total; // return number actually sent here
			return n==-1? (TcpErrorCode::FAILURE):(TcpErrorCode::SUCCESS);
		}
		
		TcpErrorCode sock_read_stream(SNET_SOCKTYPE sock, San2::Utils::CThread *thr, char *data, unsigned int dataSize, unsigned int *bytesRead, unsigned int mTimRX)
		{
			if (sock == -1) return TcpErrorCode::FAILURE;
			fd_set fds;
			// struct sockaddr_un remoteAddress;
			int selRval;
			struct timeval tv;
			
			while (1)
			{
				FD_ZERO(&fds);
				FD_SET(sock, &fds);
				
				ms2tv(mTimRX, &tv);
				
				errno = 0;
				selRval = select(sock+1, &fds, NULL, NULL, &tv);
				
				if (errno == EINTR) 
				{
					// MUST not AFFECT errno
					if (thr->isTerminated()) return TcpErrorCode::TERMINATED;
					continue;
				}
				
				if (thr->isTerminated()) return TcpErrorCode::TERMINATED;
				
				if (selRval == 0) continue; // timeout, terminated check already performed
				if (selRval < 0) return TcpErrorCode::FAILURE;
				break; // success
			}
			
			*bytesRead = recv(sock, data, dataSize, 0);
			if (*bytesRead == 0) return TcpErrorCode::PEER_DISCONNECT;
			if (*bytesRead < 0) return TcpErrorCode::FAILURE;
			return TcpErrorCode::SUCCESS;
		}
}} // ns

