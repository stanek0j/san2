
#ifndef WORKERNODEOUT_HPP_
#define WORKERNODEOUT_HPP_

#include "rpc/cirpcsyncfunctionout.hpp"

namespace San2 {
namespace Interfaces {

class WorkerNodeOut : public San2::Rpc::CIRpcSyncFunctionOut
{

public:
	WorkerNodeOut();
	unsigned int getUniqueId()const;
	bool pack(San2::Utils::bytes &out);
    bool parseResponse(const San2::Utils::bytes &in);

};

} /* namespace Interfaces */
} /* namespace San2 */
#endif /* WORKERNODEOUT_HPP_ */
