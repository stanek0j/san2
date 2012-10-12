
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif


#include "serverreceiver.hpp"

ServerReceiver::ServerReceiver(SNET_SOCKTYPE handle, unsigned long s_addr, unsigned int timRX, unsigned int timTX) :
	San2::Tcp::CTcpChannel(handle, s_addr, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{
	
}

ServerReceiver::~ServerReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Tcp::TcpErrorCode ServerReceiver::receive()
{
    printf("ServerReceiver::run()\n");
	
	m_rpcChannel = new San2::Comm::TcpRpcChannel(this);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	ret = m_rpcexec->registerFunction([](){return new Multiply();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	m_rpcexec->run();

	printf("client exit\n");
	return San2::Tcp::TcpErrorCode::SUCCESS;
}
