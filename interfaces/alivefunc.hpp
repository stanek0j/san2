
#include <memory>
#include "rpc/cirpcfunction.hpp"

namespace San2 {
	namespace Interfaces
	{
		class AliveFunc : public San2::Rpc::CIRpcFunction
		{
		public:
			AliveFunc();
			unsigned int getUniqueId()const;
			bool operator()(void);
			
			bool pack(San2::Utils::bytes &out);
			bool unpack(const San2::Utils::bytes &in);
		
		};
	}
}
