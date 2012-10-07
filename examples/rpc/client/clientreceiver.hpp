
#pragma once

#include "cppl/abstractreceiver.hpp"
#include "comm/cpplrpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "examples/rpc/testfunc.hpp"

class ClientReceiver : public San2::Cppl::AbstractReceiver
{
  public:
	ClientReceiver();
	San2::Cppl::ErrorCode run();
  protected:
	~ClientReceiver();
  private:
	 // another msvc fix
	#ifdef LINUX
		ClientReceiver(const ClientReceiver& copyFromMe)=delete;
		ClientReceiver& operator=(const ClientReceiver& copyFromMe)=delete;
	#endif
	
	San2::Comm::CpplRpcChannel m_rpcChannel;
	San2::Rpc::CRpcExecutor m_rpcexec;
};
