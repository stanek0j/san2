
#pragma once

#include <string.h>
#include <assert.h>

#ifdef WINDOWS
	#include <windows.h>
#endif

#include "utils/cthread.hpp"

#define CPPL_SUCCESS 0
#define CPPL_TIMEOUT 1
#define CPPL_TERMINATED -2
#define CPPL_ERROR -3

namespace San2
{
	namespace Cppl
	{
		enum class ErrorCode : int
		{ 
			SUCCESS = 0, 
			FAILURE = -1,
			TERMINATED = -2,
			TIMEOUT = -3, 
			PEER_DISCONNECT = -4,
			REQUEST_DISCONNECT = -5,
			ILLEGAL_CHARACTER_RECEIVED = -6,
			SEGMENT_TOO_LONG = -7,
			BUFFER_TO_SMALL = -8
		};
		
		int inline errorCodeToInt(ErrorCode code)
		{
			switch (code)
			{
			case ErrorCode::SUCCESS:
				return 0;
			case ErrorCode::FAILURE:
				return -1;
			case ErrorCode::TERMINATED:
				return -2;
			case ErrorCode::TIMEOUT:
				return -3;
			case ErrorCode::PEER_DISCONNECT:
				return -4;
			case ErrorCode::REQUEST_DISCONNECT:
				return -5;
			case ErrorCode::ILLEGAL_CHARACTER_RECEIVED:
				return -6;
			case ErrorCode::SEGMENT_TOO_LONG:
				return -7;
			case ErrorCode::BUFFER_TO_SMALL:
				return -8;
			}
			return 1; // needs fix
		}

		ErrorCode inline intToErrorCode(int code)
		{
			switch (code)
			{
			case 0:
				return ErrorCode::SUCCESS;
			case -1:
				return ErrorCode::FAILURE;
			case -2:
				return ErrorCode::TERMINATED;
			case -3:
				return ErrorCode::TIMEOUT;
			case -4:
				return ErrorCode::PEER_DISCONNECT;
			case -5:
				return ErrorCode::REQUEST_DISCONNECT;
			case -6:
				return ErrorCode::ILLEGAL_CHARACTER_RECEIVED;
			case -7:
				return ErrorCode::SEGMENT_TOO_LONG;
			case -8:
				return ErrorCode::BUFFER_TO_SMALL;
			}
			return ErrorCode::FAILURE; // needs fix
		}

		int cppl_extract_buffer(char *readBuffer, unsigned int readBufferSize, unsigned int *readBufferLen, char *outputBuffer, unsigned int outputBufferSize, unsigned int extractCount);
		int cppl_append_buffer(char *readBuffer, unsigned int readBufferSize, unsigned int *readBufferLen, char *append, unsigned int appendLen);

		#ifdef LINUX
			void ms2tv(unsigned long msec, struct timeval *tv);
			ErrorCode cppl_nix_sendall_stream(int sock, const char *buf, int *len, unsigned int timeoutMsec, San2::Utils::CThread *thr);
			ErrorCode cppl_nix_read(int sock, San2::Utils::CThread *thr, char *data, unsigned int dataSize, unsigned int *bytesRead, unsigned int mTimRX);
		#endif
		
		#ifdef WINDOWS
			ErrorCode cppl_win_sendall_stream(HANDLE hPipe, const char *buf, int *len, DWORD timeoutMsec, San2::Utils::CThread *thr); // STREAM
			ErrorCode cppl_win_read(HANDLE hPipe, San2::Utils::CThread *thr, char *data, unsigned int dataSize, DWORD *bytesRead, unsigned int mTimRX);
		#endif
	}
}
