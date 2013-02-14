
#include <algorithm>
#include "workernodeout.hpp"
#include "utils/cdatapack.hpp"

namespace San2 {
namespace Interfaces {

WorkerNodeOut::WorkerNodeOut()
{
}

unsigned int WorkerNodeOut::getUniqueId() const
{
	return 128;
}

bool WorkerNodeOut::pack(San2::Utils::bytes &out)
{
	return true;
}

bool WorkerNodeOut::parseResponse(const San2::Utils::bytes &in)
{
	return true;
}

} /* namespace Interfaces */
} /* namespace San2 */
