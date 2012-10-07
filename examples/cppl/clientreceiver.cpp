
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"

namespace San2 { namespace Cppl {
	
	ClientReceiver::ClientReceiver()
	{
		
	}
	
	ErrorCode ClientReceiver::run()
	{
		BufferProcessor *bp = getBufferProcessor();

		unsigned int bytesRead;
		const unsigned int dataSize = 512;
		char data[dataSize];

		while(bp->readSome(data, dataSize, &bytesRead) == ErrorCode::SUCCESS)
		{
			fwrite(data, 1, bytesRead, stdout);
			fflush(stdout);
		}
		printf("client exit cc \n");
		return ErrorCode::SUCCESS;
	}

    ClientReceiver::~ClientReceiver()
    {
	  // empty	
	}
	
}} // ns
