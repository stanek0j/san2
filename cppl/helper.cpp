
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif

#ifdef WIN32
    #include <WinSock2.h>
	#include <windows.h>
#endif

#include "helper.hpp"

namespace San2 { namespace Cppl {
	
        int cppl_extract_buffer(char *readBuffer, unsigned int readBufferSize, unsigned int *readBufferLen, char *outputBuffer, unsigned int outputBufferSize, unsigned int extractCount)
        {
            assert(!(extractCount == 0 || *readBufferLen == 0 || extractCount > readBufferSize || extractCount > outputBufferSize || *readBufferLen > readBufferSize || extractCount > *readBufferLen));
            if (extractCount == 0 || *readBufferLen == 0 || extractCount > readBufferSize || extractCount > outputBufferSize || *readBufferLen > readBufferSize || extractCount > *readBufferLen) return -1;
            memcpy(outputBuffer, readBuffer, extractCount);
            unsigned int remainingLen = *readBufferLen - extractCount;
            for (unsigned int i = 0; i < remainingLen; i++) readBuffer[i] = readBuffer[i + extractCount];
            *readBufferLen -= extractCount;
            return 0;
        }

        int cppl_append_buffer(char *readBuffer, unsigned int readBufferSize, unsigned int *readBufferLen, char *append, unsigned int appendLen)
        {
            assert(!(appendLen == 0 || readBufferSize < *readBufferLen + appendLen));
            if (appendLen == 0 || readBufferSize < *readBufferLen + appendLen) return -1;
            memcpy(readBuffer + *readBufferLen, append, appendLen);
            *readBufferLen += appendLen;
            return 0;
        }


	#ifdef LINUX
		// miliseconds to timeval struct
		void ms2tv(unsigned long msec, struct timeval *tv) 
		{
			tv->tv_sec = msec / 1000;
			tv->tv_usec = (msec % 1000) * 1000;
		}

		ErrorCode cppl_nix_sendall_stream(int sock, const char *buf, int *len, unsigned int timeoutMsec, San2::Utils::CThread *thr)
		{
			if (sock == -1) return ErrorCode::FAILURE;
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
					if (thr->isTerminated()) return ErrorCode::TERMINATED;
					continue; // INTERRUPT continue
				}
				
				if (thr->isTerminated()) return ErrorCode::TERMINATED;
				if (selRval == 0) continue; // timeout, terminated check already performed
				if (selRval < 0) return ErrorCode::FAILURE;
				
				errno = 0;
				n = send(sock, buf+total, bytesleft, 0);
				if (errno == EPIPE || errno == ECONNRESET) return ErrorCode::PEER_DISCONNECT;
				if (n == -1) break;
				total += n;
				bytesleft -= n;
			}

			*len = total; // return number actually sent here
			return n==-1? (ErrorCode::FAILURE):(ErrorCode::SUCCESS);
		}
		
		ErrorCode cppl_nix_read(int sock, San2::Utils::CThread *thr, char *data, unsigned int dataSize, unsigned int *bytesRead, unsigned int mTimRX)
		{
			if (sock == -1) return ErrorCode::FAILURE;
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
					if (thr->isTerminated()) return ErrorCode::TERMINATED;
					continue;
				}
				
				if (thr->isTerminated()) return ErrorCode::TERMINATED;
				
				if (selRval == 0) continue; // timeout, terminated check already performed
				if (selRval < 0) return ErrorCode::FAILURE;
				break; // success
			}
			
			*bytesRead = recv(sock, data, dataSize, 0);
			if (*bytesRead == 0) return ErrorCode::PEER_DISCONNECT;
			if (*bytesRead < 0) return ErrorCode::FAILURE;
			return ErrorCode::SUCCESS;
		}
	#endif

	#ifdef WINDOWS
		ErrorCode cppl_win_sendall_stream(HANDLE hPipe, const char *buf, int *len, DWORD timeoutMsec, San2::Utils::CThread *thr) // STREAM
		{
			if (hPipe == INVALID_HANDLE_VALUE) return ErrorCode::FAILURE;
			int total = 0;        // how many bytes we've sent
			int bytesleft = *len; // how many we have left to send
			DWORD n = 0; // =0 kvuli warningu

            OVERLAPPED ovrl;
            memset(&ovrl, 0, sizeof(OVERLAPPED)); // might be unnecessary
			ovrl.Offset = 0;
			ovrl.OffsetHigh = 0;
			ovrl.hEvent = CreateEvent(NULL, true, false, NULL);
			if (ovrl.hEvent == NULL) ovrl.hEvent = INVALID_HANDLE_VALUE;

			while(total < *len) 
			{
				if (thr->isTerminated()) return ErrorCode::TERMINATED;

				if (ResetEvent(ovrl.hEvent) == FALSE)
				{
					printf("cppl_win_send_async:ResetEvent failed\n");
					return ErrorCode::FAILURE;
				}

				WriteFile(hPipe, buf+total, bytesleft, NULL, &ovrl);

				DWORD errCode = GetLastError();
		
				if (errCode != ERROR_IO_PENDING && errCode != ERROR_SUCCESS)
				{			
                    if (errCode == ERROR_NO_DATA) return ErrorCode::PEER_DISCONNECT;
					printf("WriteFile() fatal error: GLA:%d\n", errCode);
					return ErrorCode::FAILURE;
				}

				DWORD rval = WaitForSingleObject(ovrl.hEvent, timeoutMsec);
					

				if (rval == WAIT_ABANDONED || rval == WAIT_FAILED) return ErrorCode::FAILURE;
				if (rval == WAIT_TIMEOUT) continue;
				if (rval != WAIT_OBJECT_0) return ErrorCode::FAILURE;
				
				// should not return false, because it cant be pending
				if(GetOverlappedResult(hPipe, &ovrl, &n, FALSE) == FALSE)
				{
                   DWORD lastErr = GetLastError();
                   if (lastErr == ERROR_BROKEN_PIPE) return ErrorCode::PEER_DISCONNECT;
                   printf("write: GetOverlappedResult() failed with GLA: %d\n", GetLastError());
				   return ErrorCode::FAILURE;
				} 

				if (n <= 0) return ErrorCode::FAILURE;
				
				total += n;
				bytesleft -= n;
			}

			*len = total; // return number actually sent here
            FlushFileBuffers(hPipe);
			return n==-1? (ErrorCode::FAILURE):(ErrorCode::SUCCESS);
		}	

		ErrorCode cppl_win_read(HANDLE hPipe, San2::Utils::CThread *thr, char *data, unsigned int dataSize, DWORD *bytesRead, unsigned int mTimRX)
		{	
		   if (hPipe == INVALID_HANDLE_VALUE) return ErrorCode::FAILURE;
		   OVERLAPPED o;

		   memset(&o, 0, sizeof(OVERLAPPED)); // might be unnecessary
		   o.Offset = 0;
		   o.OffsetHigh = 0;

		   // maybe second parameter false ResetEvent()???
		   o.hEvent = CreateEvent(NULL, true, false, NULL);
		   if (o.hEvent == NULL) return ErrorCode::FAILURE;

		   if (thr->isTerminated())
           {
               CloseHandle(o.hEvent);
               return ErrorCode::TERMINATED;
           }
		   
		   if (ResetEvent(o.hEvent) == FALSE)
		   {
				CloseHandle(o.hEvent);
				return ErrorCode::FAILURE;
		   }
		   
           // Read client requests from the pipe. This simplistic code only allows messages
		   // up to BUFSIZE characters in length.
		   ReadFile(hPipe, data, dataSize, NULL, &o);  

		   DWORD errCode = GetLastError();
		
		   if (errCode != ERROR_IO_PENDING && errCode != ERROR_SUCCESS)
		   {
               if (errCode == ERROR_BROKEN_PIPE) return ErrorCode::PEER_DISCONNECT;
		       printf("PipeChannel::receive()::ReadFile() fatal error: GLA:%d\n", errCode);
			   CloseHandle(o.hEvent);
			   return ErrorCode::FAILURE;
		   }

		   DWORD rval;
		   
           while(1)
		   {
				  rval = WaitForSingleObject(o.hEvent, mTimRX);
				  if (thr->isTerminated())
				  {
			  		CloseHandle(o.hEvent);
					return ErrorCode::TERMINATED;
				  }

				  if (rval == WAIT_OBJECT_0) break;
				  if (rval == WAIT_TIMEOUT) 
				  {
					if (ResetEvent(o.hEvent) == FALSE)
					{
						CloseHandle(o.hEvent);
						return ErrorCode::FAILURE;
					}  
					continue;
				  }
				  
				CloseHandle(o.hEvent);
				return ErrorCode::FAILURE;  
		    }

		 	// should not return false, because it cant be pending
			if(GetOverlappedResult(hPipe, &o, bytesRead, FALSE) == FALSE)
		    {
				 CloseHandle(o.hEvent);
                 if (GetLastError() == ERROR_BROKEN_PIPE) return ErrorCode::PEER_DISCONNECT;
				 return ErrorCode::FAILURE;
			}
			  		   
		    CloseHandle(o.hEvent);
		    //FlushFileBuffers(hPipe); 
		    return ErrorCode::SUCCESS;
		}
	#endif
}}
