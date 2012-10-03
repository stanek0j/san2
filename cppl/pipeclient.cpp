

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <functional>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/un.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
#endif

#include <string>

#include "pipeclient.hpp"
#include "helper.hpp"
#include "abstractclientreceiver.hpp"

#define CPPL_PIPECLI_RXBUFSIZE 1500
#define CPPL_PIPECLI_WIN_MAXFNAMEBYTES 512

namespace San2 { namespace Cppl {	

    void PipeClient::run()
    {
        errcode = runProc();
    }

	ErrorCode PipeClient::getErrorCode()
	{
		return errcode;
	}

	#ifdef LINUX
	
	PipeClient::PipeClient(const char *pipeName, std::function<AbstractClientReceiver* (void)> createAbstractClientReceiverProc, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
		pipename(pipeName),
		absReceiver(createAbstractClientReceiverProc()), // ugly: no error checking (excpetion new)
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
        errcode(ErrorCode::SUCCESS)
	{
		absReceiver->mClient = this;
	}

	PipeClient::~PipeClient()
	{
	    // empty
	}

    ErrorCode PipeClient::runProc()
	{
	   ErrorCode rval = absReceiver->run();    
	   close(sock);
	   sock = -1;
	   return rval;
	}

	// TODO: socket closing in open() and receive(), add cleanup() and destructor again
	ErrorCode PipeClient::open()
	{
		sock = -1;
		if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
		{
			perror("socket");
			return ErrorCode::FAILURE;
		}		

		remote.sun_family = AF_UNIX;
		strcpy(remote.sun_path, pipename);
		int len = strlen(remote.sun_path) + sizeof(remote.sun_family);
		
		if (connect(sock, (struct sockaddr *)&remote, len) == -1) 
		{
			perror("connect");
			return ErrorCode::FAILURE;
		}

		return ErrorCode::SUCCESS;
	}
	
	ErrorCode PipeClient::send(char *data, int len)
	{			
		return cppl_nix_sendall_stream(sock, data, &len, mTimTX, this);
	}
	
	ErrorCode PipeClient::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
    {
        return cppl_nix_read(sock, this, data, dataSize, bytesRead, mTimRX);
    }
	
	#endif

	#ifdef WINDOWS
	
	PipeClient::PipeClient(const char *pipeName, std::function<CPPL::AbstractClientReceiver* (void)> createAbstractClientReceiverProc, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
		pipename(pipeName),
		absReceiver(createAbstractClientReceiverProc()), // ugly: no error checking (excpetion new)
		mTimCON(timCON),
		mTimRX(timRX),
		mTimTX(timTX),
		hPipe(INVALID_HANDLE_VALUE),
        errcode(ErrorCode::SUCCESS)
	{
	   absReceiver->mClient = this;
	}

	PipeClient::~PipeClient()
	{
		delete absReceiver;
	}

	ErrorCode PipeClient::open()
	{
	   BOOL   fSuccess = FALSE; 
	   DWORD  dwMode; 
	   
	   // this means that lpszPipename is type of LPWSTR
	   wchar_t lpszPipename[CPPL_PIPECLI_WIN_MAXFNAMEBYTES]; // = TEXT("\\\\.\\pipe\\mynamedpipe"); 
 
	   if (strlen(pipename) >= CPPL_PIPECLI_WIN_MAXFNAMEBYTES) return ErrorCode::FAILURE;
	   
	   // LPCSTR -> LPCWSTR
	   if (!MultiByteToWideChar(CP_UTF8, 0, pipename, strlen(pipename) + 1, lpszPipename, CPPL_PIPECLI_WIN_MAXFNAMEBYTES))
	   {
		   printf("Error PipeServer::start()::MultiByteToWideChar::GLA: %d", GetLastError());
		   return ErrorCode::FAILURE;
	   }	  

	   // i didnt come with this crazy solution
	   // http://msdn.microsoft.com/en-us/library/windows/desktop/aa365592(v=vs.85).aspx
	   while (1) 
	   { 
		  hPipe = CreateFile( 
			 lpszPipename,   // pipe name 
			 GENERIC_READ |  // read and write access 
			 GENERIC_WRITE, 
			 0,              // no sharing 
			 NULL,           // default security attributes
			 OPEN_EXISTING,  // opens existing pipe 
			 FILE_FLAG_OVERLAPPED, // async I/O needed for full-dulpex POSIX-like I/O
			 NULL);          // no template file 
 
	   // Break if the pipe handle is valid. 
 
		  if (hPipe != INVALID_HANDLE_VALUE) break; 
 
		  // Exit if an error other than ERROR_PIPE_BUSY occurs. 
 
		  if (GetLastError() != ERROR_PIPE_BUSY) 
		  {
			 printf("Could not open pipe. GLE=%d\n", GetLastError() ); 
             CloseHandle(hPipe);
             hPipe = INVALID_HANDLE_VALUE;
			 return ErrorCode::FAILURE;
		  }
 
		  // All pipe instances are busy, so wait for mTimCON ms. 
		  if (!WaitNamedPipe(lpszPipename, mTimCON)) return ErrorCode::FAILURE;
		   
	   } 
 
	   // All pipe instances are busy, so wait for 20 seconds. 
	   // The pipe connected; change to message-read mode. 
 
	   dwMode = PIPE_READMODE_BYTE; 
	   
	   fSuccess = SetNamedPipeHandleState( 
		  hPipe,    // pipe handle 
		  &dwMode,  // new pipe mode 
		  NULL,     // don't set maximum bytes 
		  NULL);    // don't set maximum time (must be NULL on local machine)
	   
	   if (!fSuccess) 
	   {
		  printf("SetNamedPipeHandleState failed. GLE=%d\n", GetLastError()); 
		  CloseHandle(hPipe);
          hPipe = INVALID_HANDLE_VALUE;
		  return ErrorCode::FAILURE;
	   }

	   return ErrorCode::SUCCESS;
	}

    ErrorCode PipeClient::runProc()
	{
	   ErrorCode rval = absReceiver->run();    
       CloseHandle(hPipe);
       hPipe = INVALID_HANDLE_VALUE;
	   return rval;
	}

	ErrorCode PipeClient::send(char *data, int len)
	{	
        return cppl_win_sendall_stream(hPipe, data, &len, mTimTX, this);
	}

    ErrorCode PipeClient::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
    {
        return cppl_win_read(hPipe, this, data, dataSize, (DWORD*)bytesRead, mTimRX);
    }

	#endif
}} // ns
