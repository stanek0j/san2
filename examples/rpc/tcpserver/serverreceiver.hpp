
#pragma once

#include "comm/tcprpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "examples/rpc/testfunc.hpp"
#include "examples/rpc/multiply.hpp"
#include "tcp/ctcpchannel.hpp"
#include "tcp/tcphelper.hpp"

class ServerReceiver : public San2::Tcp::CTcpChannel
{
  public:
	ServerReceiver(SNET_SOCKTYPE handle, unsigned long addr, unsigned int timRX, unsigned int timTX);
	virtual ~ServerReceiver();
  protected:
	
	San2::Tcp::TcpErrorCode receive();
  private:

	 // another msvc fix
	#ifdef LINUX
		ServerReceiver(const ServerReceiver& copyFromMe)=delete;
		ServerReceiver& operator=(const ServerReceiver& copyFromMe)=delete;
	#endif
	San2::Comm::TcpRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcExecutor *m_rpcexec;
};
