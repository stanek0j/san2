
#include "ctcpinterface.hpp"

namespace San2 { namespace Interfaces {

CTcpInterface::CTcpInterface(const std::string &localIp, const std::string &localPort, const std::string &remoteIp, const std::string &remotePort, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue, unsigned long maxOutputQueueSize) :
	m_localIp(localIp),
	m_localPort(localPort),
	m_remoteIp(remoteIp),
	m_remotePort(remotePort),
	m_timeCON(timeCON),
	m_timeRX(timeRX),
	m_timeTX(timeTX),
	m_inputQueue(inputQueue),
	srv(localIp, localPort, timCON, timRX, timTX, inputQueue),
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{
	
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
	m_rpcChannel = new San2::Comm::TcpRpcChannel(this);
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

void CTcpInterface::received()
{
	// intentionally empty as the client only sends
	// we dont call client.start() so this is NEVER called (because we have overridden run())
	// but must be implemented, becouse it is a pure virtual function in CTcpClient
	// (again very confusing, sorry for that)
	return;
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



}} // ns