
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "serverreceiver.hpp"
#include "pipechannel.hpp"
#include "helper.hpp"

namespace CPPL
{
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
		
		PipeChannel *chann = getPipeChannel();
		
		while (1)
		{
			ErrorCode rval = chann->readLine(line, lineSize);
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
			
			CPPL::ErrorCode sendRval = send(line, lineLen);

			if (sendRval != CPPL::ErrorCode::SUCCESS) // echo
			{
				printf("error: srvproc send\n");
				return CPPL::ErrorCode::FAILURE;
			}
		}
		
		printf("client exit\n");
		return CPPL::ErrorCode::SUCCESS;
	}
	
}
