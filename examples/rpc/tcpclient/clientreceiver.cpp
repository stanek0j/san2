
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"
#include "comm/tcpstreamrw.hpp"

ClientReceiver::ClientReceiver(const char *ip, const char *port, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Tcp::CTcpClient(ip, port,timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpci(NULL)
{
	
}

ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpci != NULL) delete m_rpci;
}

San2::Tcp::TcpErrorCode ClientReceiver::receive()
{
	San2::Comm::TcpStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpci = new San2::Rpc::CRpcInvoker(*m_rpcChannel, 5000);
	
	bool ret = m_rpci->registerFunction([](){return new TestFuncOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	
	ret = m_rpci->registerFunction([](){return new MultiplyOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	TestFuncOut tf;
	bool x = m_rpci->invokeFunction(tf);
	if (!x) printf("Invoke fail\n");
	
	
	MultiplyOut mp(3, 5);
	x = m_rpci->invokeFunction(mp);
	if (!x) printf("Invoke fail\n");
	//---------------------------------

	unsigned int bytesRead;
	const unsigned int dataSize = 512;
	char data[dataSize];

	while(read(data, dataSize, &bytesRead) == San2::Tcp::TcpErrorCode::SUCCESS)
	{
		fwrite(data, 1, bytesRead, stdout);
		fflush(stdout);
	}
	printf("client exit cc \n");
	
	return San2::Tcp::TcpErrorCode::SUCCESS;
}

