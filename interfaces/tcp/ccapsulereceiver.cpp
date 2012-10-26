
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <algorithm>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif


#include "ccapsulereceiver.hpp"
#include "interfaces/sendcapsulefunc.hpp"
#include "rpc/cirpcfunction.hpp"
#include "comm/tcpstreamrw.hpp"
#include "utils/cvector.hpp"
#include "network/nettypedef.hpp"
#include "ctcpinterface.hpp"
#include "utils/log.h"

namespace San2 { namespace Interfaces {
	
CCapsuleReceiver::CCapsuleReceiver(CTcpInterface &iface, const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue) :
	San2::Tcp::CSingleTcpServer(ip, port, timCON, timRX, timTX),
	m_iface(iface),
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

bool CCapsuleReceiver::parseFirstMessage(const San2::Utils::bytes &data)
{
	if (data.size() != 4 + San2::Network::sanAddressSize) return false;
	
	if (data[0] != 0x53) return false; // S
	if (data[1] != 0x41) return false; // A
	if (data[2] != 0x4E) return false; // N
	if (data[3] != 0x32) return false; // 2

	San2::Network::SanAddress adr;

	std::copy(data.begin() + 4, data.end(), adr.begin());
	m_iface.setPeerAddress(adr); // thread-safe function
	return true;
}

San2::Tcp::TcpErrorCode CCapsuleReceiver::receive()
{
    printf("CCapsuleReceiver::receive()\n");
    
    San2::Comm::TcpStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([&m_inputQueue, this, &m_iface](){return new SendCapsuleFunc(m_iface.getInterfaceAddress(), &m_inputQueue, this);});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	San2::Utils::bytes firstMessage;
	
//	printf("@@@@@@@@@@@@@@@@@ 1 \n");
	if (stream.readExactNumBytesAppend(firstMessage, 4 + San2::Network::sanAddressSize) != true)
	{
		// fail, close connection
		// have in mind, that this return will not shut down the server
		// it will only close this connection and listen for another one
		// (again fault-tolerant)
		FILE_LOG(logDEBUG2) << "CCapsuleReceiver::receive(): reading first message *FAILED*";
		return San2::Tcp::TcpErrorCode::SUCCESS; // Must be SUCCESS, otherwise server will shutdown completely
	}
	
//	printf("@@@@@@@@@@@@@@@@@ 20 \n");
	
	if (parseFirstMessage(firstMessage) != true)
	{
		FILE_LOG(logDEBUG2) << "CCapsuleReceiver::receive():parseFirstMessage *FAILED*";
		return San2::Tcp::TcpErrorCode::SUCCESS; // Must be SUCCESS, otherwise server will shutdown completely
	}
	
	m_rpcexec->run();

	printf("client exit\n");
	return San2::Tcp::TcpErrorCode::SUCCESS;
}

}} // ns
