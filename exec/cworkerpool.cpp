
#include <thread>
#include "cworkerpool.hpp"

namespace San2 {
namespace Exec {

CWorkerPool::CWorkerPool()
{
	hwThreads = std::thread::hardware_concurrency();
}

const unsigned int CWorkerPool::getHwThreads()
{
	return hwThreads;
}

} /* namespace Worker */
} /* namespace San2 */