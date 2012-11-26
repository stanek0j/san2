
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

#include "pipechannel.hpp"
#include "helper.hpp"
#include "network/sockdefs.h"

#define CPPL_PIPECHANN_RXBUFSIZE 1500

namespace San2 { namespace Cppl {
	
		PipeChannel::~PipeChannel()
		{
			// empty
		}
		
		ErrorCode PipeChannel::getErrorCode()
		{
			return errcode;
		}
	
	#ifdef LINUX
		PipeChannel::PipeChannel(int sockt, unsigned int timRX, unsigned int timTX) :
			sock(sockt),
			mTimRX(timRX),
			mTimTX(timTX),
            errcode(ErrorCode::SUCCESS)
		{
			
		}
		
		// call this only once
		void PipeChannel::run()
		{
			errcode = receive();
			san_cleanup_socket(&sock);
		}

		ErrorCode PipeChannel::send(char *data, int len)
		{
			return cppl_nix_sendall_stream(sock, data, &len, mTimTX, this);
		}
		
		ErrorCode PipeChannel::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
        {
            return cppl_nix_read(sock, this, data, dataSize, bytesRead, mTimRX);
        }
		
	#endif

	#ifdef WINDOWS
		PipeChannel::PipeChannel(HANDLE handlePipe, unsigned int timRX, unsigned int timTX) :
			hPipe(handlePipe),
			mTimRX(timRX),
			mTimTX(timTX),
            errcode(ErrorCode::SUCCESS)
		{
			
		}

		// TODO: SetCommTimeouts() for isTerminated(), thread termination missing
		void PipeChannel::run()
		{	
            errcode = receive();
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
		}

        ErrorCode PipeChannel::read(char *data, unsigned int dataSize, unsigned int *bytesRead)
        {
            return cppl_win_read(hPipe, this, data, dataSize, (DWORD*)bytesRead, mTimRX);
        }

		ErrorCode PipeChannel::send(char *data, int len)
		{	
            return cppl_win_sendall_stream(hPipe, data, &len, mTimTX, this);
        }
	#endif
}} // ns
