
#include "cirpcsyncfunctionin.hpp"

namespace San2
{
	namespace Rpc
	{
			bool CIRpcSyncFunctionIn::operator() (const CIRpcSyncFunctionIn& x, const CIRpcSyncFunctionIn& y) const
			{
				return x.getUniqueId() < y.getUniqueId();
			}
	}
}
