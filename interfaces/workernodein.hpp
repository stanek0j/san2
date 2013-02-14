
#ifndef WORKERNODEIN_HPP_
#define WORKERNODEIN_HPP_

#include "rpc/cirpcsyncfunctionin.hpp"
#include "utils/platform/basictypes.hpp"

namespace San2 {
namespace Interfaces {

class WorkerNodeIn : public San2::Rpc::CIRpcSyncFunctionIn
{

public:
	WorkerNodeIn();
	unsigned int getUniqueId()const;
	bool operator()(void);
	bool unpack(const San2::Utils::bytes &in);
	San2::Utils::bytes getResponse();

private:
	San2::Utils::bytes m_response;

};

} /* namespace Interfaces */
} /* namespace San2 */
#endif /* WORKERNODEIN_HPP_ */
