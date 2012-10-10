
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


ServerReceiver::ServerReceiver() :
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{

}

ServerReceiver::~ServerReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Cppl::ErrorCode ServerReceiver::run()
{
    printf("ServerReceiver::run()\n");
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();
	m_rpcChannel = new San2::Comm::CpplRpcChannel(bp);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	ret = m_rpcexec->registerFunction([](){return new Multiply();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	m_rpcexec->run();

	printf("client exit\n");
	return San2::Cppl::ErrorCode::SUCCESS;
}
