
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"

ClientReceiver::ClientReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeClient(pipeName, timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{
	
}

ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Cppl::ErrorCode ClientReceiver::receive()
{
	m_rpcChannel = new San2::Comm::CpplRpcChannel(this);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	
	ret = m_rpcexec->registerFunction([](){return new Multiply();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	TestFunc tf;
	bool x = m_rpcexec->invokeFunction(tf);
	if (!x) printf("Invoke fail\n");
	
	
	Multiply mp(3, 5);
	x = m_rpcexec->invokeFunction(mp);
	if (!x) printf("Invoke fail\n");
	//---------------------------------

	unsigned int bytesRead;
	const unsigned int dataSize = 512;
	char data[dataSize];

	while(readSome(data, dataSize, &bytesRead) == San2::Cppl::ErrorCode::SUCCESS)
	{
		fwrite(data, 1, bytesRead, stdout);
		fflush(stdout);
	}
	printf("client exit cc \n");
	
	return San2::Cppl::ErrorCode::SUCCESS;
}

