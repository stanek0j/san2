
#include <memory>
#include "rpc/cirpcfunctionin.hpp"

namespace San2 {
	namespace Interfaces
	{
		class AliveFuncIn : public San2::Rpc::CIRpcFunctionIn
		{
		public:
			AliveFuncIn();
			unsigned int getUniqueId()const;
			bool operator()(void);
			bool unpack(const San2::Utils::bytes &in);
		};
	}
}
