
#ifndef CWORKERCHANNEL_H_
#define CWORKERCHANNEL_H_

#include "cppl/pipechannel.hpp"
#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcexecutor.hpp"

namespace San2 {
namespace Node {

class CWorkerChannel : public San2::Cppl::PipeChannel
{

public:
	CWorkerChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX);
	virtual ~CWorkerChannel();

protected:
	San2::Cppl::ErrorCode receive();

private:

	 // another msvc fix
	#ifdef LINUX
		CWorkerChannel(const CWorkerChannel& copyFromMe)=delete;
		CWorkerChannel& operator=(const CWorkerChannel& copyFromMe)=delete;
	#endif
	San2::Comm::StreamRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcExecutor *m_rpcexec;

};

} /* namespace Node */
} /* namespace San2 */
#endif /* CWORKERCHANNEL_H_ */
