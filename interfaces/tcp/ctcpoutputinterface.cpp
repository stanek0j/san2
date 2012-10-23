
#include <stdlib.h>
#include <memory>
#include "ctcpoutputinterface.hpp"
#include "interfaces/sendcapsulefunc.hpp"
#include "tcp/tcphelper.hpp"
#include "network/ccapsule.hpp"
#include "utils/platform/sleep.hpp"
#include "network/nettypedef.hpp"

namespace San2 { namespace Interfaces {

// timeRX is practically ignored, as no receive operation will be in progress
CTcpOutputInterface::CTcpOutputInterface(const std::string &ip, const std::string &port, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, unsigned long maxQueueSize) :
	San2::Tcp::CTcpClient(ip, port, timeCON, timeRX, timeTX),
	m_outputQueue(maxQueueSize),
	m_timeCON(timeCON),
	m_timeRX(timeRX),
	m_timeTX(timeTX)
{

}

CTcpOutputInterface::~CTcpOutputInterface()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

void CTcpOutputInterface::up()
{
	start();
}

// this overwrites TcpClient completely, its ugly and should be avoided in future
void CTcpOutputInterface::run()
{
	
	printf("CTcpOutputInterface::run()\n");
	m_rpcChannel = new San2::Comm::TcpRpcChannel(this);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	bool ret = m_rpcexec->registerFunction([this](){return new SendCapsuleFunc(NULL, this);});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	SendCapsuleFunc func(NULL, this);
	
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

void CTcpOutputInterface::received()
{
	// intentionally empty
	return;
}

bool CTcpOutputInterface::sendCapsule(std::shared_ptr<San2::Network::CCapsule> &capsule, San2::Utils::CThread *thr)
{
	// push to output queue
	// parameter must be calling thread!
	int r = m_outputQueue.try_push<int>(capsule, thr, m_duration);
	return r == 0;
}

San2::Network::SanAddress CTcpOutputInterface::getInterfaceAddress()
{
	
}

San2::Utils::CThread* CTcpOutputInterface::getSenderThread()
{
	return this;
}
		
}} // ns
