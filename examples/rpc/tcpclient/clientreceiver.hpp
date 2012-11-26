
#pragma once

#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcinvoker.hpp"
#include "examples/rpc/testfuncout.hpp"
#include "examples/rpc/multiplyout.hpp"
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
	
	San2::Comm::StreamRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcInvoker *m_rpci;
};
