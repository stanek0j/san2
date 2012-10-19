
#include "ctcpinputinterface.hpp"
#include "ccapsulereceiver.hpp"
#include "tcp/ctcpchannel.hpp"

namespace San2 { namespace Interfaces {

CTcpInputInterface::CTcpInputInterface(const std::string &ip, const std::string &port, unsigned int timeCON, unsigned int timeRX, unsigned int timeTX, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> >& inputQueue) :
	m_ip(ip),
	m_port(port),
	m_timeCON(timeCON),
	m_timeRX(timeRX),
	m_timeTX(timeTX),
	m_inputQueue(inputQueue),
	tcpsrv(m_ip, m_port, [&m_inputQueue](SNET_SOCKTYPE sock, unsigned long addr, unsigned int timRX, unsigned int timTX){return new CCapsuleReceiver(sock, addr, timRX, timTX, m_inputQueue);}, m_timeCON, m_timeRX, m_timeTX)
{
	
}

San2::Utils::CThread* CTcpInputInterface::getReceiverThread()
{
	return &tcpsrv;
}


/*
San2::Tcp::CTcpChannel* CCapsuleReceiver_createChannel(SNET_SOCKTYPE sock, unsigned long addr, unsigned int timRX, unsigned int timTX)
{
	return new CCapsuleReceiver(sock, addr, timRX, timTX, m_inputQueue);
}
* */

void CTcpInputInterface::up()
{
	tcpsrv.start();
}


}} // ns
