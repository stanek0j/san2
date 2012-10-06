
#include "cirpcfunction.hpp"

namespace San2
{
	namespace Rpc
	{
			bool CIRpcFunction::operator() (const CIRpcFunction& x, const CIRpcFunction& y) const
			{
				return x.getUniqueId() < y.getUniqueId();
			}
	}
}
