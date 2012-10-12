
#pragma once

#include "comm/tcprpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "examples/rpc/testfunc.hpp"
#include "examples/rpc/multiply.hpp"
#include "tcp/ctcpclient.hpp"

class ClientReceiver : public San2::Tcp::CTcpClient
{
  public:
	ClientReceiver(const char *ip, const char *port, unsigned int timCON, unsigned int timRX, unsigned int timTX);
	virtual ~ClientReceiver();
	San2::Tcp::TcpErrorCode receive();
	
  private:
	 // another msvc fix
	#ifdef LINUX
		ClientReceiver(const ClientReceiver& copyFromMe)=delete;
		ClientReceiver& operator=(const ClientReceiver& copyFromMe)=delete;
	#endif
	
	San2::Comm::TcpRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcExecutor *m_rpcexec;
};
