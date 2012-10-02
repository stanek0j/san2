
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"

namespace CPPL
{
	ClientReceiver::ClientReceiver()
	{
		
	}
	
	ErrorCode ClientReceiver::run()
	{
		CPPL::PipeClient *client = getPipeClient();

		unsigned int bytesRead;
		const unsigned int dataSize = 512;
		char data[dataSize];

		while(client->readSome(data, dataSize, &bytesRead) == CPPL::ErrorCode::SUCCESS)
		{
			fwrite(data, 1, bytesRead, stdout);
			fflush(stdout);
		}
		printf("client exit cc \n");
		return CPPL::ErrorCode::SUCCESS;
	}

    ClientReceiver::~ClientReceiver()
    {
	  // empty	
	}
	
}
