
#pragma once
#include "cppl/pipechannel.hpp"
#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"
#include "examples/rpc/testfuncin.hpp"
#include "examples/rpc/multiplyin.hpp"

class ServerReceiver : public San2::Cppl::PipeChannel
{
  public:
	ServerReceiver(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX);
	virtual ~ServerReceiver();
  protected:
	
	San2::Cppl::ErrorCode receive();
  private:

	 // another msvc fix
	#ifdef LINUX
		ServerReceiver(const ServerReceiver& copyFromMe)=delete;
		ServerReceiver& operator=(const ServerReceiver& copyFromMe)=delete;
	#endif
	San2::Comm::StreamRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcExecutor *m_rpcexec;
};
