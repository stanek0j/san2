
#include <memory>
#include "rpc/cirpcfunctionout.hpp"

namespace San2 {
	namespace Interfaces
	{
		class AliveFuncOut : public San2::Rpc::CIRpcFunctionOut
		{
		public:
			AliveFuncOut();
			unsigned int getUniqueId()const;
			bool pack(San2::Utils::bytes &out);
		};
	}
}
