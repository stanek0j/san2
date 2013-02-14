
#include <string>
#include "cworker.hpp"

namespace San2 {
namespace Exec {

Worker::Worker(std::string workerID) :
		workerID(workerID)
{
}

std::string Worker::getWorkerID()
{
	return workerID;
}

} /* namespace Worker */
} /* namespace San2 */
