
#pragma once

#include <string.h>
#include <assert.h>

#include "network/sockdefs.h"
#include "utils/cthread.hpp"

namespace San2
{
	namespace Tcp
	{
		enum class TcpErrorCode : int
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
		
		int inline errorCodeToInt(TcpErrorCode code)
		{
			switch (code)
			{
			case TcpErrorCode::SUCCESS:
				return 0;
			case TcpErrorCode::FAILURE:
				return -1;
			case TcpErrorCode::TERMINATED:
				return -2;
			case TcpErrorCode::TIMEOUT:
				return -3;
			case TcpErrorCode::PEER_DISCONNECT:
				return -4;
			case TcpErrorCode::REQUEST_DISCONNECT:
				return -5;
			case TcpErrorCode::ILLEGAL_CHARACTER_RECEIVED:
				return -6;
			case TcpErrorCode::SEGMENT_TOO_LONG:
				return -7;
			case TcpErrorCode::BUFFER_TO_SMALL:
				return -8;
			}
			return 1; // needs fix
		}

		TcpErrorCode inline intToTcpErrorCode(int code)
		{
			switch (code)
			{
			case 0:
				return TcpErrorCode::SUCCESS;
			case -1:
				return TcpErrorCode::FAILURE;
			case -2:
				return TcpErrorCode::TERMINATED;
			case -3:
				return TcpErrorCode::TIMEOUT;
			case -4:
				return TcpErrorCode::PEER_DISCONNECT;
			case -5:
				return TcpErrorCode::REQUEST_DISCONNECT;
			case -6:
				return TcpErrorCode::ILLEGAL_CHARACTER_RECEIVED;
			case -7:
				return TcpErrorCode::SEGMENT_TOO_LONG;
			case -8:
				return TcpErrorCode::BUFFER_TO_SMALL;
			}
			return TcpErrorCode::FAILURE; // needs fix
		}

		void ms2tv(unsigned long msec, struct timeval *tv);
		TcpErrorCode sock_sendall_stream(SNET_SOCKTYPE sock, const char *buf, int *len, unsigned int timeoutMsec, San2::Utils::CThread *thr);
		TcpErrorCode sock_read_stream(SNET_SOCKTYPE sock, San2::Utils::CThread *thr, char *data, unsigned int dataSize, unsigned int *bytesRead, unsigned int mTimRX);
		
	}
}
