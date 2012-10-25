
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
	m_rpcexec(NULL)
{
	
}

ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Tcp::TcpErrorCode ClientReceiver::receive()
{
	San2::Comm::TcpStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);
	
	bool ret = m_rpcexec->registerFunction([](){return new TestFunc();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	
	ret = m_rpcexec->registerFunction([](){return new Multiply();});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	TestFunc tf;
	bool x = m_rpcexec->invokeFunction(tf);
	if (!x) printf("Invoke fail\n");
	
	
	Multiply mp(3, 5);
	x = m_rpcexec->invokeFunction(mp);
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

