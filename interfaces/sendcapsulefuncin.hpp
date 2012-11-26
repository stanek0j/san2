
#include <memory>
#include "rpc/cirpcfunctionin.hpp"
#include "utils/cproducerconsumer.hpp"
#include "network/ccapsule.hpp"
#include "network/nettypedef.hpp"
#include "utils/cvector.hpp"

namespace San2 {
	namespace Interfaces
	{
		class SendCapsuleFuncIn : public San2::Rpc::CIRpcFunctionIn
		{
		public:
			static const int timeoutMilisec;
		
			SendCapsuleFuncIn(const San2::Network::SanAddress &interfaceAddress, San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *inputQueue, San2::Utils::CThread* thr);
			unsigned int getUniqueId()const;
			bool operator()(void);
			bool unpack(const San2::Utils::bytes &in);
		private:
			const San2::Network::SanAddress &m_interfaceAddress;
			San2::Utils::bytes m_SerializedCapsule;
			San2::Utils::CProducerConsumer<std::shared_ptr<San2::Network::CCapsule> > *m_inputQueue;
			San2::Utils::CThread* m_thr;
		};
	}
}
