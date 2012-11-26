
#pragma once

#include "cppl/pipeclient.hpp"
#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcinvoker.hpp"
#include "examples/rpc/testfuncout.hpp"
#include "examples/rpc/multiplyout.hpp"

class ClientReceiver : public San2::Cppl::PipeClient
{
  public:
	ClientReceiver(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX);
	virtual ~ClientReceiver();
	San2::Cppl::ErrorCode receive();
	
  private:
	 // another msvc fix
	#ifdef LINUX
		ClientReceiver(const ClientReceiver& copyFromMe)=delete;
		ClientReceiver& operator=(const ClientReceiver& copyFromMe)=delete;
	#endif
	
	San2::Comm::StreamRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcInvoker *m_rpci;
};
