
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"
	
ClientReceiver::ClientReceiver() :
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{
	
}


ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}
	

San2::Cppl::ErrorCode ClientReceiver::run()
{
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();
	m_rpcChannel = new San2::Comm::CpplRpcChannel(bp);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	TestFunc tf;
	bool x = m_rpcexec->invokeFunction(tf);
	if (!x) printf("Invoke fail\n");
	//---------------------------------

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

