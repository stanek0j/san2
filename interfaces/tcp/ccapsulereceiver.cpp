
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
#include "interfaces/sendcapsulefuncin.hpp"
#include "interfaces/alivefuncin.hpp"
#include "rpc/cirpcfunctionin.hpp"
#include "comm/tcpstreamrw.hpp"
#include "utils/cvector.hpp"
#include "network/nettypedef.hpp"
#include "ctcpinterface.hpp"
#include "utils/log.h"

namespace San2 { namespace Interfaces {
	
CCapsuleReceiver::CCapsuleReceiver(CTcpInterface &iface, const std::string &ip, const std::string &port, unsigned int timCON, unsigned int timRX, unsigned int timTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue, San2::Utils::Atomic<San2::Network::InterfaceLineStatus> &RXstatus) :
	San2::Tcp::CSingleTcpServer(ip, port, timCON, timRX, timTX),
	m_iface(iface),
	m_rpcChannel(NULL),
	m_rpcexec(NULL),
	m_inputQueue(inputQueue),
    m_RXstatus(RXstatus)
{
	m_RXstatus = San2::Network::InterfaceLineStatus::DISCONNECTED;
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
    m_RXstatus = San2::Network::InterfaceLineStatus::DISCONNECTED;
    FILE_LOG(logDEBUG4) << "IFACE-TCP: start RX";
    
    San2::Comm::TcpStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
    // Another MSVC fix :)
    // error C3480: a lambda capture variable must be from an enclosing function scope
    San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& msvc_fix_inputQueue = m_inputQueue;
    CTcpInterface &msvc_fix_iface = m_iface;

	bool ret = m_rpcexec->registerFunction([&msvc_fix_inputQueue, this, &msvc_fix_iface](){return new SendCapsuleFuncIn(m_iface.getInterfaceAddress(), &m_inputQueue, this);});
	if (!ret)
	{
		FILE_LOG(logERROR) << "CCapsuleReceiver::receive(): registrer function *FAILED*";
        m_RXstatus = San2::Network::InterfaceLineStatus::FAILURE;
		return San2::Tcp::TcpErrorCode::FAILURE;
	}	
	
    ret = m_rpcexec->registerFunction([](){return new AliveFuncIn;});
	if (!ret)
	{
		FILE_LOG(logERROR) << "CTcpInterface::run(): registrer AliveFunc function FAILED";
        m_RXstatus = San2::Network::InterfaceLineStatus::FAILURE;
		return San2::Tcp::TcpErrorCode::FAILURE;
	}	

	San2::Utils::bytes firstMessage;
	
	if (stream.readExactNumBytesAppend(firstMessage, 4 + San2::Network::sanAddressSize) != true)
	{
		// fail, close connection
		// have in mind, that this return will not shut down the server
		// it will only close this connection and listen for another one
		// (again fault-tolerant)
		FILE_LOG(logDEBUG2) << "CCapsuleReceiver::receive(): reading first message *FAILED*";
		return San2::Tcp::TcpErrorCode::SUCCESS; // Must be SUCCESS, otherwise server will shutdown completely
	}
	
	if (parseFirstMessage(firstMessage) != true)
	{
		FILE_LOG(logDEBUG2) << "CCapsuleReceiver::receive():parseFirstMessage *FAILED*";
		return San2::Tcp::TcpErrorCode::SUCCESS; // Must be SUCCESS, otherwise server will shutdown completely
	}
	
    m_RXstatus = San2::Network::InterfaceLineStatus::CONNECTED;
	m_rpcexec->run();
    m_RXstatus = San2::Network::InterfaceLineStatus::DISCONNECTED;

    m_iface.setPeerAddress(San2::Network::sanDefaultAddress); // set to zero

	// FILE_LOG(logDEBUG4) << "CCapsuleReceiver::receive(): returned";
	return San2::Tcp::TcpErrorCode::SUCCESS;
}

}} // ns
