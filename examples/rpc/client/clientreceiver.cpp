
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"
	
ClientReceiver::ClientReceiver() :
	m_rpcChannel(getBufferProcessor()),
	m_rpcexec(m_rpcChannel, 5000)
{
	
}

San2::Cppl::ErrorCode ClientReceiver::run()
{
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();

	unsigned int bytesRead;
	const unsigned int dataSize = 512;
	char data[dataSize];

	while(bp->readSome(data, dataSize, &bytesRead) == San2::Cppl::ErrorCode::SUCCESS)
	{
		fwrite(data, 1, bytesRead, stdout);
		fflush(stdout);
	}
	printf("client exit cc \n");
	return San2::Cppl::ErrorCode::SUCCESS;
}

ClientReceiver::~ClientReceiver()
{
  // empty	
}
	
