
#include <memory>
#include "rpc/cirpcfunction.hpp"
#include "utils/cproducerconsumer.hpp"
#include "network/ccapsule.hpp"
#include "network/nettypedef.hpp"
#include "utils/cvector.hpp"

namespace San2 {
	namespace Interfaces
	{
		class SendCapsuleFunc : public San2::Rpc::CIRpcFunction
		{
		public:
			static const int timeoutMilisec;
		
			SendCapsuleFunc(const San2::Network::SanAddress &interfaceAddress, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread* thr);
			bool setCapsuleToSend(std::shared_ptr<San2::Network::CCapsule> capsule);
			unsigned int getUniqueId()const;
			bool operator()(void);
			
			bool pack(San2::Utils::bytes &out);
			bool unpack(const San2::Utils::bytes &in);
		private:
			const San2::Network::SanAddress &m_interfaceAddress;
			San2::Utils::bytes m_SerializedCapsule;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *m_inputQueue;
			San2::Utils::CThread* m_thr;
		};
	}
}
