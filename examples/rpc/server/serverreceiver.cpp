
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
	San2::Cppl::BufferProcessor *bp = getBufferProcessor();
	m_rpcChannel = new San2::Comm::CpplRpcChannel(bp);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	m_rpcexec->run();
	
	const unsigned int lineSize = 512;
	char line[lineSize];

	while (1)
	{
		San2::Cppl::ErrorCode rval = bp->readLine(line, lineSize);
		if (rval != San2::Cppl::ErrorCode::SUCCESS)
		{
			if (rval == San2::Cppl::ErrorCode::PEER_DISCONNECT)
			{
				printf("peer disconnected\n");
				return San2::Cppl::ErrorCode::PEER_DISCONNECT;
			}
			printf("readLine failed errcode: %d\n", errorCodeToInt(rval));
			
			break;
		}
		
		unsigned int lineLen = strlen(line); 
		printf("rxline: %d\n", lineLen);
		
		line[strlen(line)] = 0x0A;
		lineLen++;
		
		San2::Cppl::ErrorCode sendRval = send(line, lineLen);

		if (sendRval != San2::Cppl::ErrorCode::SUCCESS) // echo
		{
			printf("error: srvproc send\n");
			return San2::Cppl::ErrorCode::FAILURE;
		}
	}

	printf("client exit\n");
	return San2::Cppl::ErrorCode::SUCCESS;
}
