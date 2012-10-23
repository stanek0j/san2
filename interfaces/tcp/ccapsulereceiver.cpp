
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif


#include "ccapsulereceiver.hpp"
#include "interfaces/sendcapsulefunc.hpp"
#include "rpc/cirpcfunction.hpp"

namespace San2 { namespace Interfaces {
	
CCapsuleReceiver::CCapsuleReceiver(const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue) :
	San2::Tcp::CSingleTcpServer(ip, port, timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpcexec(NULL),
	m_inputQueue(inputQueue)
{
	
}

CCapsuleReceiver::~CCapsuleReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Tcp::TcpErrorCode CCapsuleReceiver::receive()
{
    printf("CCapsuleReceiver::receive()\n");
    
	m_rpcChannel = new San2::Comm::TcpRpcChannel(this);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([&m_inputQueue, this](){return new SendCapsuleFunc(&m_inputQueue, this);});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	m_rpcexec->run();

	printf("client exit\n");
	return San2::Tcp::TcpErrorCode::SUCCESS;
}

}} // ns
