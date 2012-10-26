
#include "ctcpinterface.hpp"
#include "interfaces/sendcapsulefunc.hpp"
#include "utils/platform/sleep.hpp"
#include "comm/tcpstreamrw.hpp"
#include "utils/cvector.hpp"
#include "utils/log.h"

namespace San2 { namespace Interfaces {
		

CTcpInterface::CTcpInterface(San2::Network::SanAddress sanaddr, const std::string &localIp, const std::string &localPort, const std::string &remoteIp, const std::string &remotePort, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue, unsigned long maxOutputQueueSize) :
	San2::Tcp::CTcpClient(remoteIp, remotePort, timeCON, timeRX, timeTX),
	m_sanaddr(sanaddr),
	m_peeraddr(San2::Network::sanDefaultAddress),
	m_localIp(localIp),
	m_localPort(localPort),
	m_remoteIp(remoteIp),
	m_remotePort(remotePort),
	m_timeCON(timeCON),
	m_timeRX(timeRX),
	m_timeTX(timeTX),
	m_inputQueue(inputQueue),
	m_outputQueue(maxOutputQueueSize),
	srv(self() ,localIp, localPort, timeCON, timeRX, timeTX, inputQueue),
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{
	
}

San2::Utils::bytes CTcpInterface::firstMessage(const San2::Network::SanAddress& addr)
{
	// Todo: fix 4
	San2::Utils::bytes firstMessage;
	firstMessage.resize(4 + San2::Network::sanAddressSize);
	firstMessage[0] = 0x53; // S
	firstMessage[1] = 0x41; // A
	firstMessage[2] = 0x4E; // N
	firstMessage[3] = 0x32; // 2
	std::copy(addr.begin(), addr.end(), firstMessage.begin() + 4);	
	return firstMessage;
}

San2::Utils::CThread* CTcpInterface::getThread()
{
	return this;
}

void CTcpInterface::up()
{
	start();
}

void CTcpInterface::run()
{
	srv.start(); // start sender thread, it will put received capsules in inputQueue automatically
	
	// Now the client=sender stuff
	
	printf("CTcpInterface::run()\n");
	
	San2::Comm::TcpStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	bool ret = m_rpcexec->registerFunction([this](){return new SendCapsuleFunc(NULL, this);});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	SendCapsuleFunc func(NULL, this);
	
	// get item from outputQueue and send it
	
	// Fault-tolerant client
	while(!isTerminated()) // When node fails and reconnectes, this ensures connection will be ok
	{
		if (open() != San2::Tcp::TcpErrorCode::SUCCESS)
		{
			unsigned int sleepSec = m_timeCON / 1000;
			// Safety feature
			if (sleepSec <= 0) sleepSec = 1;
			
			San2::Utils::SanSleep(sleepSec);
			continue;
		}
		
		// TODO: This could block indefinitely. Fix.
		if (stream.writeAll(firstMessage(m_sanaddr)) != true)
		{
			// conection failure, retry
			// (again, we want to have it fault-tolerant)
			continue;
		}
		
		while(!isTerminated())
		{
			std::shared_ptr<San2::Network::CCapsule> capsule;
			m_outputQueue.pop<int>(&capsule, this, m_duration);
			func.setCapsuleToSend(capsule);
			bool rval = m_rpcexec->invokeFunction(func);
			if (!rval) break;
		}
		
		printf("CTcpOutputInterface::run(): CONNECTION FAILURE\n");
	}
	
	printf("client exit\n");
}

San2::Tcp::TcpErrorCode CTcpInterface::receive()
{
	// intentionally empty as the client only sends
	// we dont call client.start() so this is NEVER called (because we have overridden run())
	// but must be implemented, becouse it is a pure virtual function in CTcpClient
	// (again very confusing, sorry for that)
	return San2::Tcp::TcpErrorCode::SUCCESS;
}

bool CTcpInterface::sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr)
{
	// push to output queue
	// parameter must be calling thread, not the clientClass itself!
	
	// Should not block too long
	// TODO: Needs fix
	int r = m_outputQueue.try_push<int>(capsule, thr, m_duration);

	return r == 0;
}

void CTcpInterface::setPeerAddress(const San2::Network::SanAddress &address)
{
	FILE_LOG(logDEBUG3) << "CTcpInterface::setPeerAddress()";
	std::lock_guard<std::mutex> lock(m_mutexPeerAddress);
	m_peeraddr = address;
}

San2::Network::SanAddress CTcpInterface::getPeerAddress()
{
	std::lock_guard<std::mutex> lock(m_mutexPeerAddress);
	return m_peeraddr; // TODO: fix
}

San2::Network::SanAddress CTcpInterface::getInterfaceAddress()
{
	return m_sanaddr;
}


}} // ns
