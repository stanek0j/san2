
#include <memory>
#include "rpc/cirpcfunction.hpp"
#include "utils/cproducerconsumer.hpp"
#include "network/ccapsule.hpp"

namespace San2 {
	namespace Interfaces
	{
		class SendCapsuleFunc : public San2::Rpc::CIRpcFunction
		{
		public:
			static const int timeoutSec;
		
			SendCapsuleFunc(San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread* thr);
			void setCapsuleToSend(std::shared_ptr<San2::Network::CCapsule> capsule);
			unsigned int getUniqueId()const;
			bool operator()(void);
			
			bool pack(San2::Utils::bytes &out);
			bool unpack(const San2::Utils::bytes &in);
		private:
			std::shared_ptr<San2::Network::CCapsule> m_capsule;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *m_inputQueue;
			std::chrono::duration<int> m_duration;
			San2::Utils::CThread* m_thr;
		};
	}
}