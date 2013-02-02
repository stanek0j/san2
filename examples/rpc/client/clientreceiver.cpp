
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "clientreceiver.hpp"
#include "comm/cpplstreamrw.hpp"

ClientReceiver::ClientReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeClient(pipeName, timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpci(NULL)
{
	
}

ClientReceiver::~ClientReceiver()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpci != NULL) delete m_rpci;
}

San2::Cppl::ErrorCode ClientReceiver::receive()
{
	San2::Comm::CpplStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpci = new San2::Rpc::CRpcInvoker(*m_rpcChannel, 5000);
	
	bool ret = m_rpci->registerFunction([](){return new TestFuncOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	
	ret = m_rpci->registerSyncFunction([](){return new MultiplyOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");
	
	// --------------------------------
	TestFuncOut tf;
	if (!m_rpci->invokeFunction(tf)) printf("Invoke fail\n");
	
	
	MultiplyOut mp(3, 5);
	if (!m_rpci->invokeSyncFunction(mp)) printf("Invoke fail\n");
    std::cout << "multiplication result is: " << (int) mp.getResult() << std::endl;
    mp.m_x = 10;
    mp.m_y = 20;

    if (!m_rpci->invokeFunction(tf)) printf("Invoke fail\n");

    if (!m_rpci->invokeSyncFunction(mp)) printf("Invoke fail\n");
    std::cout << "multiplication result is: " << (int) mp.getResult() << std::endl;
    mp.m_x = 7;
    mp.m_y = 8;
    if (!m_rpci->invokeSyncFunction(mp)) printf("Invoke fail\n");
    std::cout << "multiplication result is: " << (int) mp.getResult() << std::endl;
	
	return San2::Cppl::ErrorCode::SUCCESS;
}

