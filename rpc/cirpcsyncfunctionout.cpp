
#include "cirpcsyncfunctionout.hpp"

namespace San2
{
	namespace Rpc
	{
			bool CIRpcSyncFunctionOut::operator() (const CIRpcSyncFunctionOut& x, const CIRpcSyncFunctionOut& y) const
			{
				return x.getUniqueId() < y.getUniqueId();
			}
	}
}
