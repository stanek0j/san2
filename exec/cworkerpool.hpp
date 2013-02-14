
#ifndef CWORKERPOOL_H_
#define CWORKERPOOL_H_

namespace San2 {
namespace Exec {

class CWorkerPool {

public:
	CWorkerPool();

	const unsigned int getHwThreads();

private:
	unsigned int hwThreads;

};

} /* namespace Worker */
} /* namespace San2 */
#endif /* CWORKERPOOL_H_ */
