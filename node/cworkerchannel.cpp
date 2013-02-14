
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef LINUX
	#include <sys/types.h>
	#include <sys/un.h>
	#include <signal.h>
#endif


#include "cppl/pipechannel.hpp"
#include "cppl/helper.hpp"
#include "comm/cpplstreamrw.hpp"
#include "interfaces/workernodein.hpp"
#include "cworkerchannel.hpp"

namespace San2 {
namespace Node {

CWorkerChannel::CWorkerChannel(CPPL_PIPETYPE handle, unsigned int timRX, unsigned int timTX) :
	San2::Cppl::PipeChannel(handle, timRX, timTX),
	m_rpcChannel(NULL),
	m_rpcexec(NULL)
{

}

CWorkerChannel::~CWorkerChannel()
{
	if (m_rpcChannel != NULL) delete m_rpcChannel;
	if (m_rpcexec != NULL) delete m_rpcexec;
}

San2::Cppl::ErrorCode CWorkerChannel::receive()
{
    printf("CWorkerChannel::run()\n");

	San2::Comm::CpplStreamRW stream(2000, this);
	m_rpcChannel = new San2::Comm::StreamRpcChannel(stream);
	m_rpcexec = new San2::Rpc::CRpcExecutor(*m_rpcChannel, 5000);

	bool ret = m_rpcexec->registerSyncFunction([](){return new San2::Interfaces::WorkerNodeIn;});
	if (ret) printf("reg success\n");
	else printf("reg fail\n");

	m_rpcexec->run();

	printf("client exit\n");
	return San2::Cppl::ErrorCode::SUCCESS;
}

} /* namespace Node */
} /* namespace San2 */
