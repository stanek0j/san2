
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"
#include "comm/cpplstreamrw.hpp"

ClientReceiver::ClientReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeClient(pipeName, timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpci(NULL)
{
	
}

ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpci != NULL) delete m_rpci;
}

San2::Cppl::ErrorCode ClientReceiver::receive()
{
	San2::Comm::CpplStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpci = new San2::Rpc::CRpcInvoker(*m_rpcChannel, 5000);
	
	bool ret = m_rpci->registerFunction([](){return new TestFuncOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	
	ret = m_rpci->registerFunction([](){return new MultiplyOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	TestFuncOut tf;
	bool x = m_rpci->invokeFunction(tf);
	if (!x) printf("Invoke fail\n");
	
	
	MultiplyOut mp(3, 5);
	x = m_rpci->invokeFunction(mp);
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

