
#ifndef CWORKERRECIEVER_H_
#define CWORKERRECIEVER_H_

#include "cppl/pipeclient.hpp"
#include "comm/streamrpcchannel.hpp"
#include "rpc/crpcinvoker.hpp"

namespace San2 {
namespace Worker {

class CWorkerReciever: public San2::Cppl::PipeClient
{

public:
	CWorkerReciever(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX);
	virtual ~CWorkerReciever();
	San2::Cppl::ErrorCode receive();

private:
	 // another msvc fix
	#ifdef LINUX
		CWorkerReciever(const CWorkerReciever& copyFromMe)=delete;
		CWorkerReciever& operator=(const CWorkerReciever& copyFromMe)=delete;
	#endif

	San2::Comm::StreamRpcChannel *m_rpcChannel;
	San2::Rpc::CRpcInvoker *m_rpci;

};

} /* namespace Worker */
} /* namespace San2 */
#endif /* CWORKERRECIEVER_H_ */
