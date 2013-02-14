
#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <string>

namespace San2 {
namespace Exec {

class Worker
{

public:
	Worker(std::string workerID);

	std::string getWorkerID();

protected:

private:
	std::string workerID;
	//TODO ipc channel

};

} /* namespace Worker */
} /* namespace San2 */
#endif /* WORKER_HPP_ */
