
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

#include "rpc/crpcexecutor.hpp"

#include "serverreceiver.hpp"

ServerReceiver::ServerReceiver(CIRpcChannel& channel)
{
	
}

ServerReceiver::~ServerReceiver()
{
  // empty	
}

San2::Cppl::ErrorCode ServerReceiver::run()
{
	const unsigned int lineSize = 512;
	char line[lineSize];
	
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();
	
	while (1)
	{
		ErrorCode rval = bp->readLine(line, lineSize);
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			if (rval == San2::Cppl::ErrorCode::PEER_DISCONNECT)
			{
				printf("peer disconnected\n");
				return San2::Cppl::ErrorCode::PEER_DISCONNECT;
			}
			printf("readLine failed errcode: %d\n", San2::Cppl::errorCodeToInt(rval));
			
			break;
		}
		
		unsigned int lineLen = strlen(line); 
		printf("rxline: %d\n", lineLen);
		
		line[strlen(line)] = 0x0A;
		lineLen++;
		
		San2::Cppl::ErrorCode sendRval = bp->send(line, lineLen);

		if (sendRval != San2::Cppl::ErrorCode::SUCCESS) // echo
		{
			printf("error: srvproc send\n");
			return San2::Cppl::ErrorCode::FAILURE;
		}
	}
	
	printf("client exit\n");
	return San2::Cppl::ErrorCode::SUCCESS;
}
