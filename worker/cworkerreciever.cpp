
#include <stdio.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif

#include "comm/cpplstreamrw.hpp"
#include "interfaces/workernodeout.hpp"
#include "cworkerreciever.hpp"

namespace San2 {
namespace Worker {

CWorkerReciever::CWorkerReciever(const char *pipeName, unsigned int timCON, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeClient(pipeName, timCON, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpci(NULL)
{

}

CWorkerReciever::~CWorkerReciever()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpci != NULL) delete m_rpci;
}

San2::Cppl::ErrorCode CWorkerReciever::receive()
{
	San2::Comm::CpplStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpci = new San2::Rpc::CRpcInvoker(*m_rpcChannel, 5000);

	bool ret = m_rpci->registerSyncFunction([](){return new San2::Interfaces::WorkerNodeOut;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");

	return San2::Cppl::ErrorCode::SUCCESS;
}

} /* namespace Worker */
} /* namespace San2 */
