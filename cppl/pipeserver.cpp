
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#ifdef WINDOWS
	#include <windows.h> 
#endif

#include <string>
#include <functional>

#include "pipeserver.hpp"

#define CPPL_PIPESERV_UNIX_MAXCONNQUEUE 5
#define CPPL_PIPESERV_WIN_OUTBUFSIZE 512
#define CPPL_PIPESERV_WIN_INBUFSIZE 512
// 0 = default 50 msec (MSDN)
#define CPPL_PIPESERV_WIN_CONNECTTIMEO 0
#define CPPL_PIPESERV_WIN_MAXFNAMEBYTES 512

// See lpName parametr of CreateNamedPipe() function
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa365150(v=vs.85).aspx
#define CPPL_PIPESERV_WIN_MAXCHARPIPENAME 256

namespace San2 { namespace Cppl { 

    void PipeServer::run()
    {
        errcode = runProc();
    }
    
    ErrorCode PipeServer::getErrorCode()
	{
		return errcode;
	}

	#ifdef LINUX
	
	PipeServer::PipeServer(const char *pipeName, std::function<PipeChannel* (CPPL_PIPETYPE, unsigned int, unsigned int)> proc, unsigned int timCON, unsigned int timRX, unsigned int timTX):
		pipename(pipeName),
		m_proc(proc),
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
        errcode(ErrorCode::SUCCESS)
	{
		
	}
	
	// muust be exec only once!
	ErrorCode PipeServer::runProc()
	{	
		int listenSocket = -1;
		struct sockaddr_un localAddress;		

		if ((listenSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
		{
			perror("socket");
			return ErrorCode::FAILURE;
		}
		
		localAddress.sun_family = AF_UNIX;
		strcpy(localAddress.sun_path, pipename);
		unlink(localAddress.sun_path);
		unsigned int len = strlen(localAddress.sun_path) + sizeof(localAddress.sun_family);
		
		if (bind(listenSocket, (struct sockaddr *)&localAddress, len) == -1) 
		{
			perror("bind");
			close(listenSocket);
			return ErrorCode::FAILURE;
		}
	
		/*
		 * The backlog argument defines the maximum length to which the queue of pending connections 
		 * for sockfd may  grow.   If  a  connection request  arrives  when  the queue is full, 
		 * the client may receive an error with an indication of ECONNREFUSED or, if the underlying
		 * protocol supports retransmission, the request may be ignored so that
		 * a later reattempt at connection succeeds.
		*/
		if (listen(listenSocket, CPPL_PIPESERV_UNIX_MAXCONNQUEUE) == -1) 
		{
			perror("listen");
			close(listenSocket);
			return ErrorCode::FAILURE;
		}
	
		socklen_t t;
		int acceptSocket;
		struct sockaddr_un remoteAddress;

		// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

		int tmperrno;

		// Set timeout on accept
        struct timeval tv;
        ms2tv(mTimCON, &tv);       
        setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const void *) &tv, sizeof(struct timeval));

		while(1) // TODO: Termination through IPC or SIGTERM or by closing socket
		{
			t = sizeof(sockaddr_un);
			
			errno = 0;
			acceptSocket = accept(listenSocket, (struct sockaddr *)&remoteAddress, &t);
			tmperrno = errno;
			
			if (isTerminated())
			{
				close(listenSocket);
				printf("PipeServer terminating ....\n");
				manager.terminateAndJoin();
				return ErrorCode::TERMINATED;
			}
			
			int joined = manager.joinIfFinished();
			if (joined) printf("joined %d and remains %d\n", joined, manager.threadCount());	
			
			if (tmperrno == EAGAIN)
			{
				continue;
			}

			// TODO: if timeout continue;			
			if (acceptSocket == -1) 
			{
				perror("accept");
				close(listenSocket);
				// TODO: we should here terminate and join all threads here
				return ErrorCode::FAILURE;
			}
				
			manager.startThread<PipeChannel>([=](){return m_proc(acceptSocket, mTimRX, mTimTX);});
		}
		
		return ErrorCode::SUCCESS;
	}
	#endif

	#ifdef WINDOWS
	PipeServer::PipeServer(const char *pipeName, std::function<PipeChannel* (CPPL_PIPETYPE, unsigned int, unsigned int)> proc, unsigned int timCON, unsigned int timRX, unsigned int timTX):
		pipename(pipeName),
		m_proc(proc)
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
        errcode(ErrorCode::SUCCESS)
	{
		
	}

	ErrorCode PipeServer::runProc()
	{
	   HANDLE hPipe = INVALID_HANDLE_VALUE; 
	   
	   // this means that lpszPipename is type of LPWSTR
	   wchar_t lpszPipename[CPPL_PIPESERV_WIN_MAXFNAMEBYTES]; // = TEXT("\\\\.\\pipe\\mynamedpipe"); 
	   
       if (strlen(pipename) >= CPPL_PIPESERV_WIN_MAXCHARPIPENAME) return ErrorCode::FAILURE;
	   
	   // LPCSTR -> LPCWSTR
	   if (!MultiByteToWideChar(CP_UTF8, 0, pipename, strlen(pipename) + 1, lpszPipename, CPPL_PIPESERV_WIN_MAXFNAMEBYTES))
	   {
		   printf("Error PipeServer::start()::MultiByteToWideChar::GLA: %d", GetLastError());
           return ErrorCode::FAILURE;
	   }	  
	   
	   int joined;

	   OVERLAPPED o;

	   memset(&o, 0, sizeof(OVERLAPPED)); // might be unnecessary
	   o.Offset = 0;
	   o.OffsetHigh = 0;

	   // maybe second parameter false ResetEvent()???
	   o.hEvent = CreateEvent(NULL, true, false, NULL);
       if (o.hEvent == NULL) return ErrorCode::FAILURE;

	   // The main loop creates an instance of the named pipe and 
	   // then waits for a client to connect to it. When the client 
	   // connects, a thread is created to handle communications 
	   // with that client, and this loop is free to wait for the
	   // next client connect request. It is an infinite loop.
 
	   while(1) // TODO: Termination through IPC or "something like signal"
	   { 
		  if (isTerminated())
		  {
			CloseHandle(o.hEvent);
			CloseHandle(hPipe);  
			manager.terminateAndJoin();
            return ErrorCode::TERMINATED;
		  }

		  joined = manager.joinIfFinished();
		  if (joined) printf("joined %d and remains %d\n", joined, manager.threadCount());

		  hPipe = CreateNamedPipeW( 
			  lpszPipename,             // pipe name 
			  PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,       // read/write access, asnyc becouse of timeout implementation
			  PIPE_REJECT_REMOTE_CLIENTS | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
			  PIPE_UNLIMITED_INSTANCES, // max. instances  
			  CPPL_PIPESERV_WIN_OUTBUFSIZE,      // output buffer size 
			  CPPL_PIPESERV_WIN_INBUFSIZE,       // input buffer size 
			  CPPL_PIPESERV_WIN_CONNECTTIMEO,    // client time-out 
			  NULL);                             // default security attribute 

		  if (hPipe == INVALID_HANDLE_VALUE) 
		  {
			  printf("CreateNamedPipe failed, GLE=%d.\n", GetLastError()); 
			  CloseHandle(o.hEvent);
			  CloseHandle(hPipe);
              return ErrorCode::FAILURE;
		  }
 
		  // beware of overlapped io. Return value behaves very weird.
		  ConnectNamedPipe(hPipe, &o); 
 
		  DWORD errCode = GetLastError();
		
		  if (errCode != ERROR_IO_PENDING && errCode != ERROR_SUCCESS && errCode != ERROR_PIPE_CONNECTED)
		  {			
			printf("ConnectNamedPipe() fatal error: GLA:%d\n", errCode);
			CloseHandle(o.hEvent);
			CloseHandle(hPipe);
            return ErrorCode::FAILURE;
		  }

		  DWORD rval;
		  while(1)
		  {
			rval = WaitForSingleObject(o.hEvent, mTimCON);
			
			if (isTerminated())
			{
			 	CloseHandle(o.hEvent);
				CloseHandle(hPipe);  
				manager.terminateAndJoin();
                return ErrorCode::TERMINATED;
			}

			joined = manager.joinIfFinished();
			if (joined) printf("joined %d and remains %d\n", joined, manager.threadCount());

			if (rval == WAIT_OBJECT_0) break;
			if (rval == WAIT_TIMEOUT) 
			{
				if (ResetEvent(o.hEvent) == FALSE)
				{
					printf("PipeServer::start():2:ResetEvent failed\n");
					CloseHandle(o.hEvent);
					CloseHandle(hPipe);
                    return ErrorCode::FAILURE;
				}  
				continue;
			}

			CloseHandle(o.hEvent);
			CloseHandle(hPipe);  
            return ErrorCode::FAILURE;
		  }
				
		  // GetOverlappedResult is useless because for ConnectNamedPipe() is return value undefined
		  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms683209(v=vs.85).aspx
		  manager.startThread<PipeChannel>([=](){return m_proc(hPipe, mTimRX, mTimTX);});
	   } 

	   CloseHandle(o.hEvent);
	   CloseHandle(hPipe);  
       return ErrorCode::SUCCESS; 
	}

	#endif
}} // ns
