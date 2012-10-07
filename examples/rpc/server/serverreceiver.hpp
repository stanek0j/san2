
#pragma once
#include "cppl/abstractreceiver.hpp"
#include "comm/cpplrpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "examples/rpc/testfunc.hpp"

class ServerReceiver : public San2::Cppl::AbstractReceiver
{
  public:
	ServerReceiver();
	
  protected:
	~ServerReceiver();
	San2::Cppl::ErrorCode run();
  private:

	 // another msvc fix
	#ifdef LINUX
		ServerReceiver(const ServerReceiver& copyFromMe)=delete;
		ServerReceiver& operator=(const ServerReceiver& copyFromMe)=delete;
	#endif
	San2::Comm::CpplRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcExecutor *m_rpcexec;
};
