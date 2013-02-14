
#include <algorithm>
#include "workernodein.hpp"
#include "utils/cdatapack.hpp"

namespace San2 {
namespace Interfaces {

WorkerNodeIn::WorkerNodeIn()
{
}

unsigned int WorkerNodeIn::getUniqueId() const
{
	return 128;
}

bool WorkerNodeIn::operator()()
{
	return true;
}

bool WorkerNodeIn::unpack(const San2::Utils::bytes &in)
{
	return true;
}

San2::Utils::bytes WorkerNodeIn::getResponse()
{
	return m_response;
}

} /* namespace Interfaces */
} /* namespace San2 */
