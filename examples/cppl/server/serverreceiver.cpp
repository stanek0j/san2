
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif


#include "cppl/pipechannel.hpp"
#include "cppl/helper.hpp"

#include "serverreceiver.hpp"

namespace San2 { namespace Cppl {
	
	ServerReceiver::ServerReceiver()
	{
		
	}
	
    ServerReceiver::~ServerReceiver()
    {
	  // empty	
	}
	
	ErrorCode ServerReceiver::run()
	{
		const unsigned int lineSize = 512;
		char line[lineSize];
		
		BufferProcessor *bp = getBufferProcessor();
		
		while (1)
		{
			ErrorCode rval = bp->readLine(line, lineSize);
			if (rval != ErrorCode::SUCCESS)
			{
				if (rval == ErrorCode::PEER_DISCONNECT)
				{
					printf("peer disconnected\n");
					return ErrorCode::PEER_DISCONNECT;
				}
				printf("readLine failed errcode: %d\n", errorCodeToInt(rval));
				
				break;
			}
			
			unsigned int lineLen = strlen(line); 
			printf("rxline: %d\n", lineLen);
			
			line[strlen(line)] = 0x0A;
			lineLen++;
			
			ErrorCode sendRval = send(line, lineLen);

			if (sendRval != ErrorCode::SUCCESS) // echo
			{
				printf("error: srvproc send\n");
				return ErrorCode::FAILURE;
			}
		}
		
		printf("client exit\n");
		return ErrorCode::SUCCESS;
	}
	
}} // ns
